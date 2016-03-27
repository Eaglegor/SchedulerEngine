#include <sch_test_all>

#include <Services/Routing/TspLibRoutingService/TspLibRoutingService.h>
#include <Persistence/SceneLoaders/TspLibSceneLoader/TspLibSceneLoader.h>
#include <chrono>

#include <cmath>

std::vector<std::string> light_datasets
{
	"Light/att48",
	"Light/bayg29",
	"Light/bays29",
	"Light/berlin52",
	"Light/br17",
	"Light/brazil58",
	"Light/burma14",
	"Light/dantzig42",
	"Light/eil51",
	"Light/eil76",
	"Light/fri26",
	"Light/ft53",
	"Light/ft70",
	"Light/ftv33",
	"Light/ftv35",
	"Light/ftv38",
	"Light/ftv44",
	"Light/ftv47",
	"Light/ftv55",
	"Light/ftv64",
	"Light/ftv70",
	"Light/gr17",
	"Light/gr21",
	"Light/gr24",
	"Light/gr48",
	"Light/gr96",
	"Light/hk48",
	"Light/p43",
	"Light/pr76",
	"Light/rat99",
	"Light/ry48p",
	"Light/st70",
	"Light/swiss42",
	"Light/ulysses16",
	"Light/ulysses22"
};

std::vector<std::string> medium_datasets
{
	"Medium/a280",
	"Medium/bier127",
	"Medium/brg180",
	"Medium/ch130",
	"Medium/ch150",
	"Medium/d198",
	"Medium/d493",
	"Medium/eil101",
	"Medium/fl417",
	"Medium/ftv170",
	"Medium/gil262",
	"Medium/gr120",
	"Medium/gr137",
	"Medium/gr202",
	"Medium/gr229",
	"Medium/gr431",
	"Medium/kro124p",
	"Medium/kroA100",
	"Medium/kroA150",
	"Medium/kroA200",
	"Medium/kroB100",
	"Medium/kroB150",
	"Medium/kroB200",
	"Medium/kroC100",
	"Medium/kroD100",
	"Medium/kroE100",
	"Medium/lin105",
	"Medium/lin318",
	"Medium/pcb442",
	"Medium/pr107",
	"Medium/pr124",
	"Medium/pr136",
	"Medium/pr144",
	"Medium/pr152",
	"Medium/pr226",
	"Medium/pr264",
	"Medium/pr299",
	"Medium/pr439",
	"Medium/rat195",
	"Medium/rbg323",
	"Medium/rbg358",
	"Medium/rbg403",
	"Medium/rbg443",
	"Medium/rd100",
	"Medium/rd400",
	"Medium/si175",
	"Medium/ts225",
	"Medium/tsp225",
	"Medium/u159"
};

std::vector<std::string> heavy_datasets
{
	"Heavy/ali535",
	"Heavy/att532",
	"Heavy/d657",
	"Heavy/gr666",
	"Heavy/p654",
	"Heavy/pa561",
	"Heavy/rat575",
	"Heavy/rat783",
	"Heavy/si535",
	"Heavy/u574",
	"Heavy/u724"
};

std::vector<std::string> huge_datasets
{
	"Huge/d1291",
	"Huge/d1655",
	"Huge/d2103",
	"Huge/dsj1000",
	"Huge/fl1400",
	"Huge/fl1577",
	"Huge/fl3795",
	"Huge/fnl4461",
	"Huge/nrw1379",
	"Huge/pcb1173",
	"Huge/pcb3038",
	"Huge/pr1002",
	"Huge/pr2392",
	"Huge/rl1304",
	"Huge/rl1323",
	"Huge/rl1889",
	"Huge/rl5915",
	"Huge/rl5934",
	"Huge/si1032",
	"Huge/u1060",
	"Huge/u1432",
	"Huge/u1817",
	"Huge/u2152",
	"Huge/u2319",
	"Huge/vm1084",
	"Huge/vm1748"
};


void runTspLibTest(const std::vector<std::string> &datasets)
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
        std::cout << "############# Testing Simple 2opt solver ####################" << std::endl;
		SimpleTwoOptTSPSolver *tsp_solver = strategy->createTSPSolver<SimpleTwoOptTSPSolver>();
		tsp_solver->setScheduleCostFunction(cost_function);
		solver = tsp_solver;
		acceptable_optimum_deviation = 0.5;
    }

    SECTION("SATwoOpt")
    {
        std::cout << "############# Testing SA 2opt solver ####################" << std::endl;
        SATwoOptTSPSolver *tsp_solver = strategy->createTSPSolver<SATwoOptTSPSolver>();
        tsp_solver->setScheduleCostFunction(cost_function);
        tsp_solver->setAcceptanceFunction(new BasicAcceptanceFunction(100.f, 1.f, 1.f));
        solver = tsp_solver;
        acceptable_optimum_deviation = 0.5;
    }

	float max_deviation = 0;

	for (const std::string& dataset : datasets)
	{
		Cost cost;
		uint32_t optimal_value;

		std::cout << std::endl;
		std::cout << "#### Dataset: " << dataset << " ####" << std::endl;
	
		long milliseconds = 0;

		for (size_t i = 0; i < 10; ++i)
        {
            Scene* scene = scene_loader.loadScene(std::string(TSPLIB_BENCHMARK_DATA_ROOT) + "/" + dataset + ".bin", &routing_service, TspLibSceneLoader::Format::BINARY, optimal_value);

			std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
			solver->optimize(scene->getSchedules()[0]);
			std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

			std::chrono::milliseconds local_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			long count = local_duration.count();
			milliseconds += count;

			cost = cost_function->calculateCost(scene->getSchedules()[0]);
		}

		float deviation = std::fabs(cost.getValue() - optimal_value) / static_cast<float>(optimal_value);
		if (deviation > max_deviation) max_deviation = deviation;

		std::cout << "# Known optimal cost: " << optimal_value << std::endl;
		std::cout << "# Average time (10 runs): " << milliseconds / 10 << " ms" << std::endl;
		std::cout << "# Cost: " << cost.getValue() << std::endl;
		std::cout << "# Deviation from optimal cost: " << deviation * 100 << " % " << (deviation > acceptable_optimum_deviation ? "(Unoptimal!!!)" : "") << std::endl;
		std::cout << "#### End of dataset ####" << std::endl;
		std::cout << std::endl;
	}

	REQUIRE(max_deviation < acceptable_optimum_deviation);
}



TEST_CASE("Benchmark - TspLib - Light", "[benchmark][tsp][tsplib_light]")
{
	runTspLibTest(light_datasets);
}

TEST_CASE("Benchmark - TspLib - Medium", "[benchmark][tsp][tsplib_medium]")
{
	runTspLibTest(medium_datasets);
}

TEST_CASE("Benchmark - TspLib - Heavy", "[benchmark][tsp][.tsplib_heavy]")
{
	runTspLibTest(heavy_datasets);
}

TEST_CASE("Benchmark - TspLib - Huge", "[benchmark][tsp][.tsplib_huge]")
{
	runTspLibTest(huge_datasets);
}
