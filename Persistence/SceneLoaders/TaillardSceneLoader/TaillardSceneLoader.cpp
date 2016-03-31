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

namespace Scheduler
{

	TaillardSceneLoader::TaillardSceneLoader(SceneManager* scene_manager, RoutingService* rs):
	scene_manager(scene_manager),
	rs(rs)
	{
	}

	Scene* TaillardSceneLoader::loadScene(std::istream& stream, uint32_t &out_known_optimum)
	{
		return nullptr;
	}

	Scene* TaillardSceneLoader::loadScene(const std::string& filename, uint32_t &out_known_optimum)
	{
		std::ifstream file;
		file.open(filename.c_str());
		assert(file.is_open());
		Scene* scene = loadScene(file, out_known_optimum);
		file.close();
		return scene;
	}
}
