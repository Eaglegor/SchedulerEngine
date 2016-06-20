#include <catch.hpp>

#include <Persistence/StrategyLoaders/JSONStrategyLoader/JSONStrategyLoader.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/Engine/Engine.h>

TEST_CASE("Persistence - StrategyLoaders - JSONStrategyLoader", "[integration][functional][persistence]")
{
	boost::property_tree::ptree props;
	using namespace Scheduler;

	CrowFlyRoutingService rs;
	Engine engine;

	StrategiesManager* sm = engine.getStrategiesManager();

	std::ifstream ifile;
	ifile.open("TestData/JSONStrategyLoader_Test/TestStrategy1.json");
	REQUIRE(ifile.is_open());

	JSONStrategyLoader loader(sm);
	Strategy* strategy = loader.loadStrategy(ifile);

	REQUIRE(strategy);

	ifile.close();
}
