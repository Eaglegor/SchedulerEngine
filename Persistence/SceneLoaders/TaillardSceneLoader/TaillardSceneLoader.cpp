#include "TaillardSceneLoader.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <string>

#include <Engine/Algorithms/RunVehicleBinders/PerformerAssigned/PerformerAssignedVehicleBinder.h>
#include <Engine/Algorithms/ScheduleActualization/Route/Default/DefaultRouteActualizationAlgorithm.h>
#include <Engine/SceneManager/Depot.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/WorkOperation.h>

#include <Engine/Algorithms/Validation/Run/Overload/OverloadRunValidationAlgorithm.h>
#include <Engine/Algorithms/Validation/Schedule/ValidRuns/ValidRunsScheduleValidationAlgorithm.h>
#include <Engine/Core/Engine.h>
#include <Engine/SceneManager/ScheduleValidationModel.h>

namespace Scheduler
{

	TaillardSceneLoader::TaillardSceneLoader(Engine& engine, RoutingService& rs)
	    : engine(engine),
	      rs(rs)
	{
	}

	Scene& TaillardSceneLoader::loadScene(std::istream& stream)
	{
		SceneContext& scene_context = engine.getSceneManager( ).createSceneContext( );

		std::size_t customers_number;
		float best_solution;
		float vehicle_capacity;
		std::int32_t xdepot, ydepot;

		std::size_t customer_number;
		std::int32_t x, y;
		float demand;

		stream >> customers_number >> best_solution >> vehicle_capacity >> xdepot >> ydepot;

		Site depot_location;
		depot_location.setLatitude(Coordinate(xdepot));
		depot_location.setLongitude(Coordinate(ydepot));

		PerformerAssignedVehicleBinder& vehicle_binder = engine.getAlgorithmsManager( ).createAlgorithm<PerformerAssignedVehicleBinder>( );

		ScheduleActualizationModel actualization_model;
		DefaultRouteActualizationAlgorithm& route_actualization_algorithm = engine.getAlgorithmsManager( ).createAlgorithm<DefaultRouteActualizationAlgorithm>(rs);
		actualization_model.setRouteActualizationAlgorithm(route_actualization_algorithm);

		ScheduleValidationModel validation_model;
		ScheduleValidationAlgorithm& valid_runs_algorithm = engine.getAlgorithmsManager( ).createAlgorithm<ValidRunsScheduleValidationAlgorithm>( );
		RunValidationAlgorithm& overload_check_algorithm  = engine.getAlgorithmsManager( ).createAlgorithm<OverloadRunValidationAlgorithm>( );
		validation_model.setScheduleValidationAlgorithm(valid_runs_algorithm);
		validation_model.setRunValidationAlgorithm(overload_check_algorithm);

		const Depot& depot = scene_context.createDepot(scene_context.createLocation(depot_location));

		for(size_t i = 0; i < customers_number; ++i)
		{
			Vehicle& vehicle = scene_context.createVehicle( );
			{
				vehicle.constraints( ).depot( ).set(depot);
				std::string name = "Vehicle" + std::to_string(i);
				vehicle.setName(name);
				vehicle.constraints( ).capacity( ).set(Capacity(vehicle_capacity, 0, 0, 0));
			}
			Performer& performer = scene_context.createPerformer( );
			{
				performer.constraints( ).depot( ).set(depot);
				std::string name = "Driver" + std::to_string(i);
				performer.setName(name);
			}
			vehicle_binder.assign(performer, vehicle);

			stream >> customer_number >> x >> y >> demand;
			Location& location = scene_context.createLocation(Site(Coordinate(x), Coordinate(y)));

			Order& order = scene_context.createOrder(location);
			{
				std::string name = "Order" + std::to_string(i);
				order.setName(name);

				WorkOperation& work_operation = order.getWorkOperation( );
				{
					std::string op_name = name + ".Work";
					work_operation.setName(op_name);
					work_operation.constraints( ).demand( ).set(Capacity(demand, 0, 0, 0));
				}
			}
		}

		Scene& scene = engine.getSceneManager( ).createScene(scene_context);

		for(const Performer& performer : scene.getContext( ).getPerformers( ))
		{
			std::string name   = "Schedule of " + performer.getName( );
			Schedule& schedule = scene.createSchedule(performer);
			schedule.setName(name.c_str( ));

			schedule.setRunVehicleBinder(vehicle_binder);
			schedule.setActualizationModel(actualization_model);
			schedule.setValidationModel(validation_model);
		}

		return scene;
	}

	Scene& TaillardSceneLoader::loadScene(const std::string& filename)
	{
		std::ifstream file;
		file.open(filename.c_str( ));
		assert(file.is_open( ));
		Scene& scene = loadScene(file);
		file.close( );
		return scene;
	}
}
