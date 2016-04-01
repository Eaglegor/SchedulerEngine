//#include <sch_test_all>
#include <sch_scene_management>
#include <sch_strategies>
#include <sch_core>
#include <sch_cost_functions>
#include <sch_tsp_solvers>


#include <Services/Routing/TspLibRoutingService/TspLibRoutingService.h>
#include <Persistence/SceneLoaders/TspLibSceneLoader/TspLibSceneLoader.h>
#include <chrono>

#include <cmath>
#include <iostream>

#include "../Publishers/StdoutBenchmarkPublisher.h"
#include "../Publishers/MarkdownBenchmarkPublisher.h"

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

enum TspSolverType
{
	GREEDY,
	TWO_OPT,
	GREEDY_AND_TWO_OPT,
	SA,
	GREEDY_AND_SA,
	SA_AND_TWO_OPT
};

void runTspLibTest(const std::vector<std::string> &datasets, Scheduler::BenchmarkPublisher& publisher, TspSolverType solver_type)
{
	using namespace Scheduler;

	TspLibRoutingService routing_service;
	EngineContext engine_context;
	engine_context.routing_service = &routing_service;
	Engine engine(engine_context);

	TspLibSceneLoader scene_loader(engine.getSceneManager());

	Strategy* strategy = engine.getStrategiesManager()->createStrategy();

	TotalDistanceScheduleCostFunction* cost_function = strategy->createScheduleCostFunction<TotalDistanceScheduleCostFunction>();

	float acceptable_optimum_deviation = 0;

	Scheduler::BenchmarkResult result;
	
	float max_deviation = 0;

	Scheduler::TSPSolver* solver;

	switch(solver_type)
	{
		case GREEDY:
		{
			std::cout << "############# Testing Greedy solver ####################" << std::endl;
			GreedyTSPSolver *tsp_solver = strategy->createTSPSolver<GreedyTSPSolver>();
			tsp_solver->setRoutingService(&routing_service);
			solver = tsp_solver;
			acceptable_optimum_deviation = 0.5;
			result.algorithm_name = "Greedy";
			break;
		}
		case GREEDY_AND_TWO_OPT:
		{
			std::cout << "############# Testing Chain: Greedy + TwoOpt ####################" << std::endl;
			ChainTSPSolver *tsp_solver = strategy->createTSPSolver<ChainTSPSolver>();

			GreedyTSPSolver *greedy_solver = strategy->createTSPSolver<GreedyTSPSolver>();
			greedy_solver->setRoutingService(&routing_service);

			SimpleTwoOptTSPSolver *two_opt_solver = strategy->createTSPSolver<SimpleTwoOptTSPSolver>();
			two_opt_solver->setScheduleCostFunction(cost_function);

			tsp_solver->addTSPSolver(greedy_solver);
			tsp_solver->addTSPSolver(two_opt_solver);

			solver = tsp_solver;
			acceptable_optimum_deviation = 0.5;
			result.algorithm_name = "Chain: Greedy + TwoOpt";
			break;
		}
		case SA_AND_TWO_OPT:
		{
			std::cout << "############# Testing Chain: SA + TwoOpt ####################" << std::endl;
			ChainTSPSolver *tsp_solver = strategy->createTSPSolver<ChainTSPSolver>();

			SATwoOptTSPSolver *sa_solver = strategy->createTSPSolver<SATwoOptTSPSolver>();
			sa_solver->setScheduleCostFunction(cost_function);
			sa_solver->setAcceptanceFunction(new BasicAcceptanceFunction());
			//sa_solver->setAcceptanceFunction(new FastAcceptanceFunction());
			sa_solver->setTemperatureFunction(new LinearTemperatureFunction(100.f, 0.1f, 0.05f));
			//sa_solver->setTemperatureFunction(new PowerTemperatureFunction(100.f, 0.1f, 0.99f));

			SimpleTwoOptTSPSolver *two_opt_solver = strategy->createTSPSolver<SimpleTwoOptTSPSolver>();
			two_opt_solver->setScheduleCostFunction(cost_function);

			tsp_solver->addTSPSolver(sa_solver);
			tsp_solver->addTSPSolver(two_opt_solver);

			solver = tsp_solver;
			acceptable_optimum_deviation = 0.5;
			result.algorithm_name = "Chain: SA + TwoOpt";
			break;
		}
	}

	size_t index = 1;

	Scheduler::BenchmarkResult total_cost;
	total_cost.dataset_name = "Summary";
	total_cost.algorithm_name = result.algorithm_name;
	total_cost.cost = 0;
	total_cost.optimal_cost = 0;
	total_cost.average_time = 0;

	for (const std::string& dataset : datasets)
	{
		Cost cost;
		uint32_t optimal_value;

		std::cout << "Running " << index ++ << "/" << datasets.size() << ": " << dataset << std::endl;
		result.dataset_name = dataset;
	
		long long nanoseconds = 0;

		for (size_t i = 0; i < 10; ++i)
        {
            Scene* scene = scene_loader.loadScene(std::string(TSPLIB_BENCHMARK_DATA_ROOT) + "/" + dataset + ".bin", &routing_service, TspLibSceneLoader::Format::BINARY, optimal_value);

			std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
			solver->optimize(scene->getSchedules()[0]);
			std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

			std::chrono::nanoseconds local_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
			long long count = local_duration.count();
			nanoseconds += count;

			cost = cost_function->calculateCost(scene->getSchedules()[0]);
		}

		result.cost = cost.getValue();
		result.optimal_cost = optimal_value;
		result.average_time = nanoseconds / 10000000.0f;
		
		total_cost.cost += result.cost;
		total_cost.optimal_cost += result.optimal_cost;
		total_cost.average_time += result.average_time;

		float deviation = std::fabs(cost.getValue() - optimal_value) / static_cast<float>(optimal_value);
		if (deviation > max_deviation) max_deviation = deviation;

		publisher.addResult(result);
	}

	publisher.addResult(total_cost);
}

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		Scheduler::MarkdownBenchmarkPublisher publisher(argv[1]);
		runTspLibTest(light_datasets, publisher, GREEDY);
		runTspLibTest(light_datasets, publisher, GREEDY_AND_TWO_OPT);
		runTspLibTest(light_datasets, publisher, SA_AND_TWO_OPT);
		publisher.publish();
	}
	else
	{
		Scheduler::StdoutBenchmarkPublisher publisher;
		runTspLibTest(light_datasets, publisher, GREEDY);
		runTspLibTest(light_datasets, publisher, GREEDY_AND_TWO_OPT);
		runTspLibTest(light_datasets, publisher, SA_AND_TWO_OPT);
		publisher.publish();
	}
}
