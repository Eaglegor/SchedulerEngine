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

namespace Scheduler
{
    AnchorInsertionVRPSolver::AnchorInsertionVRPSolver()
	{
	}
	
    AnchorInsertionVRPSolver::~AnchorInsertionVRPSolver()
    {
    }
	
	struct Insertion
	{
		Order* order;
		Run::WorkStopIterator position;
		Cost estimated_cost;
	};
	
	class InsertionSuggestor
	{
		public:
			InsertionSuggestor(Scene &scene):scene(scene){}
		
			Optional<Insertion> suggestInsertion()
			{
				return None;
			}
			
			void reset(){}
			
		private:
			Scene& scene;
	};

	class InsertionActor
	{
		public:
			InsertionActor(Scene &scene, SceneEditor& scene_editor, const SceneCostFunction& cost_function):scene(scene),scene_editor(scene_editor),cost_function(cost_function){}
			bool apply(const Insertion& insertion, Cost current_best_cost){return false;}
			
		private:
			Scene& scene;
			SceneEditor& scene_editor;
			const SceneCostFunction& cost_function;
	};
	
	struct Anchor
	{
		Order* order;
		Cost estimated_cost;
	};
	
	class AnchorSuggestor
	{
		public:
			AnchorSuggestor(Scene& scene):scene(scene){}
		
			Optional<Anchor> suggestAnchor()
			{
				return None;
			}
			
		private:
			Scene& scene;
	};
	
	class AnchorActor
	{
		public:
			AnchorActor(Scene& scene, SceneEditor& scene_editor):scene(scene),scene_editor(scene_editor){}
			bool apply(const Anchor& anchor){return false;}
			
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
		bool finished = false;
		std::size_t iterations_count = 0;		
		
		Cost current_best_cost = cost_function->calculateCost(scene);
		
		SceneEditor scene_editor;
		
		AnchorSuggestor anchor_suggestor(scene);
		AnchorActor anchor_actor(scene, scene_editor);
		
		InsertionSuggestor insertion_suggestor(scene);
		InsertionActor insertion_actor(scene, scene_editor, cost_function.get());
		
		RepairActor repair_actor(scene, scene_editor, repair_algorithm.get());
		
		while(!finished)
		{
			Optional<Anchor> anchor = anchor_suggestor.suggestAnchor();
			
			Optional<Insertion> insertion = insertion_suggestor.suggestInsertion();
			while(insertion && insertion->estimated_cost < anchor->estimated_cost)
			{
				bool successful_insertion = insertion_actor.apply(insertion.get(), current_best_cost);
				
				insertion = insertion_suggestor.suggestInsertion();
				if(successful_insertion) {
					repair_actor.apply();
					anchor = anchor_suggestor.suggestAnchor();
				}
			}
			
			bool successful_anchor = anchor_actor.apply(anchor.get());
			if(successful_anchor) {
				repair_actor.apply();
				insertion_suggestor.reset();
			}
			
			if(iterations_limit != 0 && ++iterations_count >= iterations_limit) finished = true;
		}
	}
}
