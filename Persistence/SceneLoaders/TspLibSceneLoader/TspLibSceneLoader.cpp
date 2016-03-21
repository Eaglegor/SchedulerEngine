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

	Scene* TspLibSceneLoader::loadScene(std::istream& stream, TspLibRoutingService* routing_service, Format format, uint32_t &out_known_optimum)
	{
		size_t nodes_count;

		switch(format)
		{
		case Format::BINARY:
			loadBinaryMatrix(stream, nodes_count, routing_service, out_known_optimum);
			break;
		case Format::XML:
			loadXmlMatrix(stream, nodes_count, routing_service, out_known_optimum);
		}

		Scene* scene = scene_manager->createScene();

		Vehicle* vehicle = scene->createVehicle();
		vehicle->setName("Vehicle");

		Performer* performer = scene->createPerformer();
		performer->setName("Performer");

		Schedule* schedule = scene->createSchedule(performer);

		Run *run = schedule->createRun(Location(), Location()); // Creating run (0,0) -> (0,0)
		run->setVehicle(vehicle);

		for (size_t i = 1; i < nodes_count; ++i)
		{
			Operation* operation = scene->createFreeOperation();
			operation->setName((std::string("Operation") + std::to_string(i)).c_str());
			operation->setDuration(Duration(0));
			operation->setLocation(Location(Coordinate(i), Coordinate(0))); // Index is encoded in the latitude. The longitude will be unused.
			run->allocateWorkOperation(operation);
		}

		return scene;
	}

	Scene* TspLibSceneLoader::loadScene(const std::string& filename, TspLibRoutingService* routing_service, Format format, uint32_t &out_known_optimum)
	{
		std::ifstream file;
		file.open(filename.c_str(), format == Format::BINARY ? std::ios_base::in | std::ios_base::binary : std::ios_base::in);
		assert(file.is_open());
		Scene* scene = loadScene(file, routing_service, format, out_known_optimum);
		file.close();
		return scene;
	}

	void TspLibSceneLoader::loadBinaryMatrix(std::istream& stream, size_t &out_count, TspLibRoutingService* routing_service, uint32_t &out_known_optimum)
	{
		uint32_t count;
		uint32_t optimum;
		uint32_t ignoredDigits;

		stream.read(reinterpret_cast<char*>(&count), sizeof(uint32_t));
		stream.read(reinterpret_cast<char*>(&optimum), sizeof(uint32_t));
		stream.read(reinterpret_cast<char*>(&ignoredDigits), sizeof(uint32_t));

		routing_service->init(count);

		uint32_t buffer;
		for (size_t i = 0; i < count; ++i)
		{
			for (size_t j = 0; j < count; ++j)
			{
				stream.read(reinterpret_cast<char*>(&buffer), sizeof(uint32_t));
				float distance = static_cast<float>(buffer) / pow(10, ignoredDigits);
				routing_service->insertRoute(i, j, distance);
			}
		}

		out_known_optimum = optimum;
		out_count = count;
	}

	void TspLibSceneLoader::loadXmlMatrix(std::istream& stream, size_t &out_count, TspLibRoutingService* routing_service, uint32_t &out_known_optimum)
	{
		using boost::property_tree::ptree;
		using boost::property_tree::read_xml;

		out_known_optimum = 0;

		ptree tree;
		read_xml(stream, tree);

		ptree root = tree.get_child("travellingSalesmanProblemInstance");
		const ptree& graph = root.get_child("graph");

		size_t from_index = 0;
		routing_service->init(graph.size());

		out_count = graph.size();

		bool is_first_stop = true;
		for (const auto &viter : graph)
		{
			for (const auto &eiter : viter.second)
			{
				float distance = eiter.second.get_child("<xmlattr>").get<float>("cost");
				size_t to_index = eiter.second.get_value<size_t>();
				routing_service->insertRoute(from_index, to_index, distance);
			}
			++from_index;
		}
	}
}