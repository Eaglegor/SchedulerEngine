#include "TspLibSceneLoader.h"

#include <string>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <Services/Routing/TspLibRoutingService/TspLibRoutingService.h>

#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>

namespace Scheduler
{

	TspLibSceneLoader::TspLibSceneLoader(SceneManager* scene_manager):
	scene_manager(scene_manager)
	{
	}

	Scene* TspLibSceneLoader::loadScene(std::istream& stream, TspLibRoutingService* routing_service)
	{
		using boost::property_tree::ptree;
		using boost::property_tree::read_xml;

		ptree tree;

		Scene* scene = scene_manager->createScene();

		Vehicle* vehicle = scene->createVehicle();
		vehicle->setName("Vehicle");

		Performer* performer = scene->createPerformer();
		performer->setName("Performer");

		Schedule* schedule = scene->createSchedule(performer);

		Run *run = schedule->createRun(Location(), Location()); // Creating run (0,0) -> (0,0)

		read_xml(stream, tree);

		ptree root = tree.get_child("travellingSalesmanProblemInstance");

		std::string name = root.get<std::string>("name");
		std::string source = root.get<std::string>("source");
		std::string description = root.get<std::string>("description");
		
		const ptree& graph = root.get_child("graph");
		size_t from_index = 0;
		routing_service->init(graph.size());

		std::vector<Operation*> operations;

		bool is_first_stop = true;
		for(const auto &viter : graph)
		{
			if(is_first_stop)
			{
				is_first_stop = false;
			}
			else
			{
				Operation* operation = scene->createFreeOperation();
				operation->setName((std::string("Operation") + std::to_string(from_index)).c_str());
				operation->setDuration(Duration(0));
				operation->setLocation(Location(Coordinate(from_index), Coordinate(0))); // Index is encoded in the latitude. The longitude will be unused.
				operations.push_back(operation);
			}
			for(const auto &eiter : viter.second)
			{
				float distance = eiter.second.get_child("<xmlattr>").get<float>("cost");
				size_t to_index = eiter.second.get_value<size_t>();
				routing_service->insertRoute(from_index, to_index, distance);
			}
			++from_index;
		}

		for(Operation* operation : operations)
		{
			run->allocateWorkOperation(operation);
		}

		return scene;
	}

	Scene* TspLibSceneLoader::loadScene(const std::string& filename, TspLibRoutingService* routing_service)
	{
		std::ifstream file;
		file.open(filename.c_str());
		assert(file.is_open());
		Scene* scene = loadScene(file, routing_service);
		file.close();
		return scene;
	}
}
