#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <Persistence/SceneLoaders/JSONSceneLoader/JSONSceneLoader.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <Plugins/RoutingServices/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/SceneManager.h>

TEST_CASE("Persistence - SceneLoaders - JSONSceneLoader - ModelLoading", "[unit][functional][persistence]")
{
	boost::property_tree::ptree props;
	using namespace Scheduler;

	CrowFlyRoutingService rs;
	SceneManager sm(&rs);

	std::ifstream ifile;
	ifile.open("TestData/IFTest_JSONSceneLoader/TestScene1.json");
	REQUIRE(ifile.is_open());
	
	JSONSceneLoader loader(&sm);
	Scene* scene = loader.loadScene(ifile);

	ifile.close();
}
