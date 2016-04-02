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

const char* COST_KPI_NAME = "Cost";
const char* AVERAGE_TIME_KPI_NAME = "Average time (ms)";

using namespace Scheduler;

class TspLibTestInstance
{
public:
	virtual ~TspLibTestInstance(){}

	TspLibTestInstance(const std::vector<std::string> &datasets, BenchmarkPublisher& publisher):
		publisher(publisher),
		datasets(datasets)
	{
		EngineContext engine_context;
		engine_context.routing_service = &routing_service;
		engine.reset(new Engine(engine_context));

		strategy = engine->getStrategiesManager()->createStrategy();
		cost_function = strategy->createScheduleCostFunction<TotalDistanceScheduleCostFunction>();
	}

	virtual TSPSolver* createTSPSolver(Strategy* strategy) = 0;
	virtual const char* getAlgorithmName() = 0;

	void run()
	{
		std::cout << "############# Testing solver: "  << getAlgorithmName() << " ####################" << std::endl;

		total_cost = 0;
		total_time = 0;
		for (size_t i = 0; i < datasets.size(); ++i)
		{
			runDataset(i);
		}

		BenchmarkResult result;
		result.algorithm_name = getAlgorithmName();
		result.dataset_name = "Summary";
		result.kpi.emplace(COST_KPI_NAME, std::to_string(total_cost));
		if(total_time > FLOAT_EPSILON) result.kpi.emplace(AVERAGE_TIME_KPI_NAME, std::to_string(total_time));
		publisher.addResult(result);
	}

protected:

	virtual void runDataset(size_t id)
	{
		TspLibSceneLoader scene_loader(engine->getSceneManager());

		Scheduler::BenchmarkResult result;
		result.algorithm_name = getAlgorithmName();

		TSPSolver* solver = createTSPSolver(strategy);

		Cost cost;
		uint32_t optimal_value;

		std::cout << "Running " << id + 1 << "/" << datasets.size() << ": " << datasets[id] << " ";
		result.dataset_name = datasets[id];

		long long nanoseconds = 0;

		for (size_t i = 0; i < 10; ++i)
		{
			Scene* scene = scene_loader.loadScene(std::string(TSPLIB_BENCHMARK_DATA_ROOT) + "/" + datasets[id] + ".bin", &routing_service, TspLibSceneLoader::Format::BINARY, optimal_value);

			std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
			solver->optimize(scene->getSchedules()[0]);
			std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

			std::chrono::nanoseconds local_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
			long long count = local_duration.count();
			nanoseconds += count;

			cost = cost_function->calculateCost(scene->getSchedules()[0]);

			std::cout << "#";
		}

		std::cout << std::endl;

		float deviation = std::fabs(cost.getValue() - optimal_value) / static_cast<float>(optimal_value);

		result.kpi.emplace(COST_KPI_NAME, std::to_string(cost.getValue()) + " (" + std::to_string(deviation * 100) + "%) ");
		result.kpi.emplace(AVERAGE_TIME_KPI_NAME, std::to_string(nanoseconds / 10000000.0f));

		total_cost += cost.getValue();
		total_time += nanoseconds / 10000000.0f;

		publisher.addResult(result);
	}

	float total_cost;
	float total_time;

	Scheduler::TspLibRoutingService routing_service;
	Scheduler::BenchmarkPublisher& publisher;
	std::unique_ptr<Scheduler::Engine> engine;
	Scheduler::Strategy* strategy;
	const std::vector<std::string>& datasets;
	ScheduleCostFunction* cost_function;
	
};

class Optimal_TspLibInstance : public TspLibTestInstance
{
public:
	Optimal_TspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TspLibTestInstance(datasets, publisher)
	{
	}

	virtual void runDataset(size_t id) override
	{
		TspLibSceneLoader scene_loader(engine->getSceneManager());

		Scheduler::BenchmarkResult result;
		result.algorithm_name = getAlgorithmName();

		uint32_t optimal_value;

		result.dataset_name = datasets[id];

		Scene* scene = scene_loader.loadScene(std::string(TSPLIB_BENCHMARK_DATA_ROOT) + "/" + datasets[id] + ".bin", &routing_service, TspLibSceneLoader::Format::BINARY, optimal_value);

		result.kpi.emplace(COST_KPI_NAME, std::to_string(optimal_value));

		total_cost += optimal_value;

		publisher.addResult(result);
	}

	virtual TSPSolver* createTSPSolver(Strategy* strategy) override
	{
		return nullptr;
	}

	virtual const char* getAlgorithmName() override
	{
		return "!Optimal";
	}
};

class Greedy_TspLibInstance : public TspLibTestInstance
{
public:
	Greedy_TspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TspLibTestInstance(datasets, publisher)
	{
	}

	virtual TSPSolver* createTSPSolver(Strategy* strategy) override
	{
		GreedyTSPSolver *tsp_solver = strategy->createTSPSolver<GreedyTSPSolver>();
		tsp_solver->setRoutingService(&routing_service);
		return tsp_solver;
	}

	virtual const char* getAlgorithmName() override
	{
		return "Greedy";
	}
};

class Greedy_2Opt_TspLibInstance : public TspLibTestInstance
{
public:
	Greedy_2Opt_TspLibInstance (const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TspLibTestInstance(datasets, publisher)
	{
	}

	virtual TSPSolver* createTSPSolver(Strategy* strategy) override
	{
		ChainTSPSolver *tsp_solver = strategy->createTSPSolver<ChainTSPSolver>();
		GreedyTSPSolver *greedy_solver = strategy->createTSPSolver<GreedyTSPSolver>();
		greedy_solver->setRoutingService(&routing_service);

		SimpleTwoOptTSPSolver *two_opt_solver = strategy->createTSPSolver<SimpleTwoOptTSPSolver>();
		two_opt_solver->setScheduleCostFunction(cost_function);

		tsp_solver->addTSPSolver(greedy_solver);
		tsp_solver->addTSPSolver(two_opt_solver);
		return tsp_solver;
	}

	virtual const char* getAlgorithmName() override
	{
		return "Greedy >> 2-Opt";
	}
};

class SA_2Opt_TspLibInstance : public TspLibTestInstance
{
public:
	SA_2Opt_TspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TspLibTestInstance(datasets, publisher)
	{
	}

	virtual TSPSolver* createTSPSolver(Strategy* strategy) override
	{
		ChainTSPSolver *tsp_solver = strategy->createTSPSolver<ChainTSPSolver>();

		SATwoOptTSPSolver *sa_solver = strategy->createTSPSolver<SATwoOptTSPSolver>();
		sa_solver->setScheduleCostFunction(cost_function);
		//sa_solver->setAcceptanceFunction(new BasicAcceptanceFunction());
		//sa_solver->setAcceptanceFunction(new FastAcceptanceFunction());
		//sa_solver->setTemperatureFunction(new LinearTemperatureFunction(100.f, 0.1f, 0.05f));
		//sa_solver->setTemperatureFunction(new PowerTemperatureFunction(100.f, 0.1f, 0.99f));

		SimpleTwoOptTSPSolver *two_opt_solver = strategy->createTSPSolver<SimpleTwoOptTSPSolver>();
		two_opt_solver->setScheduleCostFunction(cost_function);

		tsp_solver->addTSPSolver(sa_solver);
		tsp_solver->addTSPSolver(two_opt_solver);
		return tsp_solver;
	}

	virtual const char* getAlgorithmName() override
	{
		return "SA >> 2-Opt";
	}
};

int main(int argc, char **argv)
{
	std::unique_ptr<Scheduler::BenchmarkPublisher> publisher;
	if (argc > 1)
	{
		publisher.reset(new MarkdownBenchmarkPublisher(argv[1]));
	}
	else
	{
		publisher.reset(new StdoutBenchmarkPublisher());
	}
	
	{
		Optimal_TspLibInstance test(light_datasets, *publisher);
		test.run();
	}

	{
		Greedy_TspLibInstance test(light_datasets, *publisher);
		test.run();
	}

	{
		Greedy_2Opt_TspLibInstance test(light_datasets, *publisher);
		test.run();
	}

	{
		SA_2Opt_TspLibInstance test(light_datasets, *publisher);
		test.run();
	}

	publisher->publish();
}
