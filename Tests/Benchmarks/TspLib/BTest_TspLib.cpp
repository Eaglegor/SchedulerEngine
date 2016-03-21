#include <sch_test_all>

#include <Services/Routing/TspLibRoutingService/TspLibRoutingService.h>
#include <Persistence/SceneLoaders/TspLibSceneLoader/TspLibSceneLoader.h>

#include <cmath>

std::vector<std::string> light_datasets
{
	"att48",
	"bayg29",
	"bays29",
	"berlin52",
	"br17",
	"brazil58",
	"brg180",
	"burma14",
	"dantzig42",
	"eil51",
	"eil76",
	"fri26",
	"ft53",
	"ft70",
	"ftv33",
	"ftv35",
	"ftv38",
	"ftv44",
	"ftv47",
	"ftv55",
	"ftv64",
	"ftv70",
	"gr17",
	"gr21",
	"gr24",
	"gr48",
	"gr96",
	"hk48",
	"kro124p",
	"p43",
	"ry48p",
	"st70",
	"swiss42",
	"ulysses16",
	"ulysses22"
};

TEST_CASE("Benchmark - TspLib", "[benchmark][tsp]")
{
	using namespace Scheduler;

	TspLibRoutingService routing_service;
	EngineContext engine_context;
	engine_context.routing_service = &routing_service;
	Engine engine(engine_context);
	
	TspLibSceneLoader scene_loader(engine.getSceneManager());

	Strategy* strategy = engine.getStrategiesManager()->createStrategy();
	TSPSolver *solver;

	TotalDistanceScheduleCostFunction* cost_function = strategy->createScheduleCostFunction<TotalDistanceScheduleCostFunction>();

	float acceptable_optimum_deviation = 0;

	SECTION("SimpleTwoOpt")
	{
		SimpleTwoOptTSPSolver *tsp_solver = strategy->createTSPSolver<SimpleTwoOptTSPSolver>();
		tsp_solver->setScheduleCostFunction(cost_function);
		solver = tsp_solver;
		acceptable_optimum_deviation = 0.5;
	}

	float max_deviation = 0;

	for (const std::string& dataset : light_datasets)
	{
		uint32_t optimal_value;
		
		Scene* scene = scene_loader.loadScene(std::string(TSPLIB_BENCHMARK_LIGHT_DATA_ROOT) + "/" + dataset + ".bin", &routing_service, TspLibSceneLoader::Format::BINARY, optimal_value);

		solver->optimize(scene->getSchedules()[0]);

		Cost cost = cost_function->calculateCost(scene->getSchedules()[0]);

		float deviation = std::fabs(cost.getValue() - optimal_value) / static_cast<float>(optimal_value);

		if (deviation > max_deviation) max_deviation = deviation;

		std::cout << std::endl;
		std::cout << "#### Dataset: " << dataset << " ####" << std::endl;
		std::cout << "# Cost: " << cost.getValue() << std::endl;
		std::cout << "# Known optimal cost: " << optimal_value << std::endl;
		std::cout << "# Deviation: " << deviation * 100 << " % " << (deviation > acceptable_optimum_deviation ? "(Unoptimal!!!)" : "") << std::endl;
		std::cout << "#### End of dataset ####" << std::endl;
		std::cout << std::endl;
	}

	REQUIRE(max_deviation < acceptable_optimum_deviation);
}