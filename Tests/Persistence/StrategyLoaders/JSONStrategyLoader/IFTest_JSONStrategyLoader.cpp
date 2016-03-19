#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/JSONStrategyLoader.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/StrategiesManager/StrategiesManager.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Engine/EngineContext.h>

#include <Engine/StrategiesManager/Strategy.h>

TEST_CASE("Persistence - StrategyLoaders - JSONStrategyLoader", "[integration][functional][persistence]")
{
	boost::property_tree::ptree props;
	using namespace Scheduler;

	CrowFlyRoutingService rs;

	EngineContext context;
	context.routing_service = &rs;

	Engine engine(context);

	StrategiesManager* sm = engine.getStrategiesManager();

	std::ifstream ifile;
	ifile.open("TestData/IFTest_JSONStrategyLoader/TestStrategy1.json");
	REQUIRE(ifile.is_open());

	JSONStrategyLoader loader(sm);
	Strategy* strategy = loader.loadStrategy(ifile);

	REQUIRE(strategy);

	ifile.close();
}
