#include <catch.hpp>

#include <Services/Routing/TspLibRoutingService/TspLibRoutingService.h>
#include <Persistence/SceneLoaders/TspLibSceneLoader/TspLibSceneLoader.h>
#include <Engine/Core/Engine.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

TEST_CASE("Persistence - SceneLoaders - JSONSceneLoader", "[integration][functional][persistence]")
{
	using namespace Scheduler;

	TspLibRoutingService routing_service;

	Engine engine;

	uint32_t optimum;

	TspLibSceneLoader loader(engine);

	Optional<Scene&> scene;

	SECTION("XML")
	{
		scene = loader.loadScene("TestData/TspLibSceneLoader_Test/br17.xml", routing_service, TspLibSceneLoader::Format::XML, optimum);
	}

	SECTION("Binary")
	{
		scene = loader.loadScene("TestData/TspLibSceneLoader_Test/br17.bin", routing_service, TspLibSceneLoader::Format::BINARY, optimum);
	}
	
	REQUIRE(scene->getContext().getVehicles().size() == 1);
	REQUIRE(scene->getContext().getPerformers().size() == 1);
	REQUIRE(scene->getContext().getFreeOperations().size() == 16);
	for (size_t i = 0; i < 16; ++i)
	{
		REQUIRE(scene->getContext().getFreeOperations()[i].get().getLocation().getSite().getLatitude() == Coordinate(i + 1));
		REQUIRE(scene->getContext().getFreeOperations()[i].get().getLocation().getSite().getLongitude() == Coordinate(0));
	}
	REQUIRE(scene->getSchedules().size() == 1);
	REQUIRE(scene->getSchedules()[0].get().getRuns().size() == 1);
	REQUIRE(scene->getSchedules()[0].get().getRuns()[0].get().getWorkStops().size() == 16);
	for (size_t i = 0; i < 16; ++i)
	{
		REQUIRE(std::next(scene->getSchedules()[0].get().getRuns()[0].get().getWorkStops().begin(), i)->getOperation() == scene->getContext().getFreeOperations()[i]);
	}
}