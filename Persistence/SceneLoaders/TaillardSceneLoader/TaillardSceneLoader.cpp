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
#include <Engine/Algorithms/ScheduleActualization/Routes/RoutesActualizationAlgorithm.h>
#include <Engine/Algorithms/RunVehicleBinders/PerformerAssigned/PerformerAssignedVehicleBinder.h>

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

		for (size_t i = 0; i < customers_number; ++i)
		{
			Vehicle* vehicle = scene->createVehicle();
			{
				std::string name = "Vehicle" + std::to_string(i);
				vehicle->setName(name.c_str());
				vehicle->setCapacity(Capacity(vehicle_capacity, 0, 0, 0));
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

				schedule->getScheduleActualizer()->createAlgorithm<RoutesActualizationAlgorithm>(rs);
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
					work_operation->setDemand(Capacity(demand, 0, 0, 0));
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
