#include <sch_test_all>

#include <Services/Routing/TspLibRoutingService/TspLibRoutingService.h>
#include <Persistence/SceneLoaders/TspLibSceneLoader/TspLibSceneLoader.h>

TEST_CASE("Persistence - SceneLoaders - JSONSceneLoader", "[integration][functional][persistence]")
{
	using namespace Scheduler;

	TspLibRoutingService routing_service;

	EngineContext context;
	context.routing_service = &routing_service;

	Engine engine(context);

	SceneManager* sm = engine.getSceneManager();

	uint32_t optimum;

	TspLibSceneLoader loader(sm);

	Scene* scene = nullptr;

	SECTION("XML")
	{
		scene = loader.loadScene("TestData/IFTest_TspLibSceneLoader/br17.xml", &routing_service, TspLibSceneLoader::Format::XML, optimum);
	}

	SECTION("Binary")
	{
		scene = loader.loadScene("TestData/IFTest_TspLibSceneLoader/br17.bin", &routing_service, TspLibSceneLoader::Format::BINARY, optimum);
	}
	

	REQUIRE(scene);
	REQUIRE(scene->getVehicles().size() == 1);
	REQUIRE(scene->getPerformers().size() == 1);
	REQUIRE(scene->getFreeOperations().size() == 16);
	for (size_t i = 0; i < 16; ++i)
	{
		REQUIRE(scene->getFreeOperations()[i]->getLocation().getLatitude() == Coordinate(i + 1));
		REQUIRE(scene->getFreeOperations()[i]->getLocation().getLongitude() == Coordinate(0));
	}
	REQUIRE(scene->getSchedules().size() == 1);
	REQUIRE(scene->getSchedules()[0]->getRuns().size() == 1);
	REQUIRE(scene->getSchedules()[0]->getRuns()[0]->getWorkStops().size() == 16);
	for (size_t i = 0; i < 16; ++i)
	{
		REQUIRE(scene->getSchedules()[0]->getRuns()[0]->getWorkStops()[i]->getOperation() == scene->getFreeOperations()[i]);
	}
}
