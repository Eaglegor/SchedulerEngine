#include "AnchorInsertionVRPSolver.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Depot.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/AllocateOrder.h>
#include <Engine/SceneEditor/Actions/CreateRun.h>
#include <Engine/SceneManager/CostFunctions/SceneCostFunction.h>
#include <Engine/Routing/RoutingService.h>
#include <set>
#include <Engine/SceneManager/Vehicle.h>

namespace Scheduler
{
    AnchorInsertionVRPSolver::AnchorInsertionVRPSolver(): iterations_limit(0)
	{
	}

	AnchorInsertionVRPSolver::~AnchorInsertionVRPSolver()
    {
    }
	
	struct Insertion
	{
		const Order* order;
		Run::WorkStopIterator position;
		Cost estimated_cost;

		bool operator<(const Insertion& rhs) const
		{
			return estimated_cost < rhs.estimated_cost;
		}
	};
	
	static const float UNPLANNED_ORDER_PENALTY = 1000.0f;
	static const float VEHICLE_ACTIVATION_COST = 500.0f;

	class InsertionSuggestor
	{
		public:
			InsertionSuggestor(Scene &scene, const RoutingService& routing_service):
			scene(scene),
			routing_service(routing_service)
			{
				reset();
			}
		
			Optional<Insertion> suggestInsertion()
			{
				if (current_iterator == insertions.end()) return None;
				return *current_iterator++;
			}
			
			void reset()
			{
				insertions.clear();
				for (Schedule& schedule : scene.getSchedules())
				{
					for (Run& run : schedule.getRuns())
					{
						for (WorkStop& stop : run.getWorkStops())
						{
							for(const Order& order : scene.getContext().getOrders())
							{
								if (scene.query().operationStopMapping().findWorkStop(order.getWorkOperation().get())) continue;

								const Location &order_location = order.getWorkOperation()->getLocation();
								const Location &route_start = std::prev(run.findStop(stop))->getLocation();
								const Location &route_end = stop.getLocation();

								Insertion insertion;
								insertion.order = &order;
								insertion.position = run.findWorkStop(stop);
								
								Route ij = routing_service.calculateRoute(route_start.getSite(), route_end.getSite(), RoutingProfile() /* run.getVehicle()->getRoutingProfile()*/);
								Route ik = routing_service.calculateRoute(route_start.getSite(), order_location.getSite(), RoutingProfile() /* run.getVehicle()->getRoutingProfile()*/);
								Route kj = routing_service.calculateRoute(order_location.getSite(), route_end.getSite(), RoutingProfile() /* run.getVehicle()->getRoutingProfile()*/);

								insertion.estimated_cost = (-ij.getDuration() + ik.getDuration() + kj.getDuration()).getValue() /** schedule.getPerformer().getDurationUnitCost().getValue()*/ - UNPLANNED_ORDER_PENALTY;

								insertions.insert(insertion);
							}
						}
					}
				}

				current_iterator = insertions.begin();
			}
			
		private:
			Scene& scene;
			std::set<Insertion> insertions;
			std::set<Insertion>::iterator current_iterator;
			const RoutingService& routing_service;
	};

	class InsertionActor
	{
		public:
			InsertionActor(Scene &scene, SceneEditor& scene_editor, const SceneCostFunction& cost_function):
			scene(scene),
			scene_editor(scene_editor),
			cost_function(cost_function)
			{}

			bool apply(const Insertion& insertion, Cost current_best_cost)
			{
				Run& run = insertion.position->getRun();

				int checkpoint = scene_editor.checkpoint();
				
				scene_editor.performAction<AllocateOrder>(run, insertion.position, *insertion.order);
				
				//Cost resulting_cost = cost_function.calculateCost(scene);
				if(!run.getSchedule().isValid() /*resulting_cost > current_best_cost*/)
				{
					scene_editor.rollbackToCheckpoint(checkpoint);
					return false;
				}

				return true;
			}
			
		private:
			Scene& scene;
			SceneEditor& scene_editor;
			const SceneCostFunction& cost_function;
	};
	
	struct Anchor
	{
		const Order* order;
		Schedule* schedule;
		Cost estimated_cost;
		float estimated_density;

		bool operator<(const Anchor& rhs) const
		{
			return estimated_density > rhs.estimated_density;
		}
	};
	
	class AnchorSuggestor
	{
		public:
			AnchorSuggestor(Scene& scene, const RoutingService& routing_service):
			scene(scene),
			routing_service(routing_service)
			{
				reset();
			}
		
			Optional<Anchor> suggestAnchor()
			{
				if(current_iterator == anchors.end()) return None;
				return *current_iterator++;
			}
			
			void reset()
			{
				anchors.clear();
				for(const Order& order : scene.getContext().getOrders())
				{
					if (scene.query().operationStopMapping().findWorkStop(order.getWorkOperation().get())) continue;
					
					for(Schedule &schedule : scene.getSchedules())
					{
						if (!schedule.empty()) continue;

						Anchor anchor;
						anchor.order = &order;
						anchor.schedule = &schedule;
						anchor.estimated_density = calculateDensity(order);
						
						const Location &depot_location = schedule.getPerformer().getDepot()->getLocation();
						const Location &order_location = order.getWorkOperation()->getLocation();
						Route di = routing_service.calculateRoute(depot_location.getSite(), order_location.getSite(), RoutingProfile());
						Route id = routing_service.calculateRoute(order_location.getSite(), depot_location.getSite(), RoutingProfile());
						anchor.estimated_cost = (di.getDuration() + id.getDuration()).getValue() - UNPLANNED_ORDER_PENALTY + VEHICLE_ACTIVATION_COST;

						anchors.insert(anchor);
					}
				}
				
				current_iterator = anchors.begin();
			}

		private:
			float calculateDensity(const Order& anchor)
			{
				Duration sum_duration(0);
				Duration max_duration(0);

				for(const Order& order : scene.getContext().getOrders())
				{
					if (scene.query().operationStopMapping().findWorkStop(order.getWorkOperation().get())) continue;

					Route ij = routing_service.calculateRoute(anchor.getWorkOperation()->getLocation().getSite(), order.getWorkOperation()->getLocation().getSite(), RoutingProfile());
					sum_duration += ij.getDuration();

					if (max_duration < ij.getDuration()) max_duration = ij.getDuration();
				}

				sum_duration /= max_duration.getValue() == 0 ? 1 : max_duration.getValue();

				return 1.0f / static_cast<float>(sum_duration.getValue());
			}

			std::set<Anchor> anchors;
			std::set<Anchor>::iterator current_iterator;
			Scene& scene;
			const RoutingService &routing_service;
	};
	
	class AnchorActor
	{
		public:
			AnchorActor(Scene& scene, SceneEditor& scene_editor):
			scene(scene),
			scene_editor(scene_editor)
			{}

			bool apply(const Anchor& anchor)
			{
				Schedule& schedule = *anchor.schedule;
				const Location &depot_location = schedule.getPerformer().getDepot()->getLocation();

				int checkpoint = scene_editor.checkpoint();

				scene_editor.performAction<CreateRun>(schedule, schedule.getRuns().end(), depot_location, depot_location);
				Run& run = schedule.getRuns()[0];
				scene_editor.performAction<AllocateOrder>(run, run.getWorkStops().end(), *anchor.order);

				if(schedule.isValid())
				{
					return true;
				}
				else
				{
					scene_editor.rollbackToCheckpoint(checkpoint);
					return false;
				}
			}
			
		private:
			Scene& scene;
			SceneEditor& scene_editor;
	};

	class RepairActor
	{
		public:
			RepairActor(Scene& scene, SceneEditor& scene_editor, const VRPSolver& repair_algorithm):scene(scene),scene_editor(scene_editor),repair_algorithm(repair_algorithm){}
			bool apply(){return false;}
			
		private:
			Scene& scene;
			SceneEditor& scene_editor;
			const VRPSolver& repair_algorithm;
	};
	
	void AnchorInsertionVRPSolver::optimize(Scene& scene) const
	{
		if (!cost_function) return;
		if (!routing_service) return;

		bool finished = false;
		std::size_t iterations_count = 0;		
		
		Cost current_best_cost = cost_function->calculateCost(scene);
		
		SceneEditor scene_editor;
		
		AnchorSuggestor anchor_suggestor(scene, routing_service.get());
		AnchorActor anchor_actor(scene, scene_editor);
		
		InsertionSuggestor insertion_suggestor(scene, routing_service.get());
		InsertionActor insertion_actor(scene, scene_editor, cost_function.get());
		
		RepairActor repair_actor(scene, scene_editor, repair_algorithm.get());
		
		while(!finished)
		{
			Optional<Anchor> anchor = anchor_suggestor.suggestAnchor();
			
			Optional<Insertion> insertion = insertion_suggestor.suggestInsertion();
			while(insertion && (!anchor || insertion->estimated_cost < anchor->estimated_cost))
			{
				bool successful_insertion = insertion_actor.apply(insertion.get(), current_best_cost);
				
				if(successful_insertion) {
					scene_editor.commit();
					repair_actor.apply();
					insertion_suggestor.reset();
					anchor = anchor_suggestor.suggestAnchor();
				}

				insertion = insertion_suggestor.suggestInsertion();
			}
			
			if(anchor)
			{
				bool successful_anchor = anchor_actor.apply(anchor.get());
				if(successful_anchor) {
					scene_editor.commit();
					repair_actor.apply();
					insertion_suggestor.reset();
					anchor_suggestor.reset();
				}
			}
			
			std::cout << iterations_count << std::endl;
			
			if (!anchor && !insertion) finished = true;
			if(iterations_limit != 0 && ++iterations_count >= iterations_limit) finished = true;
		}
	}

	void AnchorInsertionVRPSolver::setRepairAlgorithm(const VRPSolver& repair_algorithm)
	{
		this->repair_algorithm = repair_algorithm;
	}

	void AnchorInsertionVRPSolver::setCostFunction(const SceneCostFunction& cost_function)
	{
		this->cost_function = cost_function;
	}

	void AnchorInsertionVRPSolver::setIterationsLimit(std::size_t iterations_limit)
	{
		this->iterations_limit = iterations_limit;
	}

	void AnchorInsertionVRPSolver::setRoutingService(const RoutingService& routing_service)
	{
		this->routing_service = routing_service;
	}
}
