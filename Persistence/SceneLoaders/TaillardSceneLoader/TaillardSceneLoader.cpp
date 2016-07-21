#include "TaillardSceneLoader.h"
#include <string>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/Algorithms/ScheduleActualization/Route/Default/DefaultRouteActualizationAlgorithm.h>
#include <Engine/Algorithms/RunVehicleBinders/PerformerAssigned/PerformerAssignedVehicleBinder.h>

#include <Engine/SceneManager/ScheduleValidationModel.h>
#include <Engine/Algorithms/Validation/Schedule/ValidRuns/ValidRunsScheduleValidationAlgorithm.h>
#include <Engine/Algorithms/Validation/Run/Overload/OverloadRunValidationAlgorithm.h>

namespace Scheduler
{

	TaillardSceneLoader::TaillardSceneLoader(SceneManager* scene_manager, RoutingService* rs):
	scene_manager(scene_manager),
	rs(rs)
	{
	}

	Scene* TaillardSceneLoader::loadScene(std::istream& stream)
	{
		Scene* scene = scene_manager->createScene();

		size_t customers_number;
		float best_solution;
		float vehicle_capacity;
		float xdepot, ydepot;

		size_t customer_number;
		float x, y, demand;

		stream >> customers_number >> best_solution >> vehicle_capacity >> xdepot >> ydepot;

		Location depot_location;
		depot_location.setLatitude(Coordinate(xdepot));
		depot_location.setLongitude(Coordinate(ydepot));

		PerformerAssignedVehicleBinder* vehicle_binder = scene->createRunVehicleBinder<PerformerAssignedVehicleBinder>();

		ScheduleActualizationModel* actualization_model = scene_manager->createScheduleActualizationModel();
		DefaultRouteActualizationAlgorithm* route_actualization_algorithm = scene_manager->createRouteActualizationAlgorithm<DefaultRouteActualizationAlgorithm>(rs);
		actualization_model->setRouteActualizationAlgorithm(route_actualization_algorithm);

		ScheduleValidationModel* validation_model = scene_manager->createScheduleValidationModel();
		ScheduleValidationAlgorithm* valid_runs_algorithm = scene_manager->createScheduleValidationAlgorithm<ValidRunsScheduleValidationAlgorithm>();
		RunValidationAlgorithm* overload_check_algorithm = scene_manager->createRunValidationAlgorithm<OverloadRunValidationAlgorithm>();
		validation_model->setScheduleValidationAlgorithm(valid_runs_algorithm);
		validation_model->setRunValidationAlgorithm(overload_check_algorithm);

		for (size_t i = 0; i < customers_number; ++i)
		{
			Vehicle* vehicle = scene->createVehicle();
			{
				std::string name = "Vehicle" + std::to_string(i);
				vehicle->setName(name.c_str());
				vehicle->constraints().capacity().set(Capacity(vehicle_capacity, 0, 0, 0));
			}
			Performer* performer = scene->createPerformer();
			{
				std::string name = "Driver" + std::to_string(i);
				performer->setName(name.c_str());
			}
			vehicle_binder->assign(performer, vehicle);
			
			Schedule* schedule = scene->createSchedule(performer);
			{
				std::string name = "Schedule of Driver" + std::to_string(i);
				schedule->setName(name.c_str());
				schedule->setDepotLocation(depot_location);

				schedule->setActualizationModel(actualization_model);
				schedule->setValidationModel(validation_model);
			}

			Order* order = scene->createOrder();
			{
				std::string name = "Order" + std::to_string(i);
				order->setName(name.c_str());
				
				stream >> customer_number >> x >> y >> demand;

				Operation* work_operation = order->createWorkOperation();
				{
					std::string op_name = name + ".Work";
					work_operation->setName(op_name.c_str());
					work_operation->constraints().demand().set(Capacity(demand, 0, 0, 0));
					work_operation->setLocation(Location(Coordinate(x), Coordinate(y)));
				}
			}
		}

		return scene;
	}

	Scene* TaillardSceneLoader::loadScene(const std::string& filename)
	{
		std::ifstream file;
		file.open(filename.c_str());
		assert(file.is_open());
		Scene* scene = loadScene(file);
		file.close();
		return scene;
	}
}
