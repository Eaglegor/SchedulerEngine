#include <numeric>


#include <Services/Routing/TspLibRoutingService/TspLibRoutingService.h>
#include <Persistence/SceneLoaders/TspLibSceneLoader/TspLibSceneLoader.h>
#include <chrono>

#include <cmath>
#include <iostream>
#include <sstream>

#include "../Publishers/StdoutBenchmarkPublisher.h"
#include "../Publishers/MarkdownBenchmarkPublisher.h"
#include <Engine/Engine/Engine.h>
#include <Engine/CostFunctions/TotalDistance/TotalDistanceScheduleCostFunction.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Engine/Algorithms/TSPSolvers/Greedy/GreedyTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/Utilitary/Chain/ChainTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/SimpleTwoOpt/SimpleTwoOptTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/HybridOpt/HybridOptTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/SimulatedAnnealing/ListTemperatureScheduler.h>
#include <Engine/Algorithms/TSPSolvers/SimulatedAnnealing/SimulatedAnnealingTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/Utilitary/BestOf/BestOfTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/Utilitary/Transparent/TransparentTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/Utilitary/Reverse/ReverseTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/Utilitary/Scramble/ScrambleTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/OneRelocate/OneRelocateTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/SuInt/SuIntTSPSolver.h>

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

const char* COST_KPI_NAME = "Mean Cost";
const char* VARIATION_KPI_NAME = "Variation";
const char* AVERAGE_TIME_KPI_NAME = "Average time (ms)";
const char* TOTAL_DEVIATION = "Deviation";

using namespace Scheduler;

class TspLibTestInstance
{
public:
	virtual ~TspLibTestInstance(){}

	TspLibTestInstance(const std::vector<std::string> &datasets, BenchmarkPublisher& publisher):
		publisher(publisher),
		datasets(datasets),
		cost_function(engine.getAlgorithmsManager().createCostFunction<TotalDistanceScheduleCostFunction>()),
		total_cost(0),
		total_time(0)
	{
	}

	virtual TSPSolver& createTSPSolver() = 0;
	virtual const char* getAlgorithmName() = 0;

	void run()
	{
		std::cout << "############# Testing solver: "  << getAlgorithmName() << " ####################" << std::endl;

        total_cost = 0.f;
		total_time = 0;
		for (size_t i = 0; i < datasets.size(); ++i)
		{
			runDataset(i);
		}

		BenchmarkResult result;
		result.algorithm_name = getAlgorithmName();
		result.dataset_name = "{Summary}";
		result.kpi.emplace(COST_KPI_NAME, std::to_string(total_cost));
		const float geometric_deviation = std::pow(std::accumulate(deviations.begin(), deviations.end(), 1.f, std::multiplies<float>()), 1.f / deviations.size());
        result.kpi.emplace(TOTAL_DEVIATION, std::to_string(geometric_deviation * 100.f - 100.f));
		if(total_time > FLOAT_EPSILON) result.kpi.emplace(AVERAGE_TIME_KPI_NAME, std::to_string(total_time));
		publisher.addResult(result);
	}

protected:

	virtual void runDataset(size_t id)
	{
		TspLibSceneLoader scene_loader(engine);

		Scheduler::BenchmarkResult result;
		result.algorithm_name = getAlgorithmName();

		TSPSolver& solver = createTSPSolver();

        std::vector<float> costs;
		uint32_t optimal_value;

        std::cout << "Running " << id + 1 << "/" << datasets.size() << ": " << datasets[id] << " " << std::flush;
		result.dataset_name = datasets[id];

		long long nanoseconds = 0;
        const size_t number_of_iterations = 10;

        for (size_t i = 0; i < number_of_iterations; ++i)
		{
			Scene& scene = scene_loader.loadScene(std::string(TSPLIB_BENCHMARK_DATA_ROOT) + "/" + datasets[id] + ".bin", routing_service, TspLibSceneLoader::Format::BINARY, optimal_value);

			std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
			solver.optimize(scene.getSchedules()[0]);
			std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

			std::chrono::nanoseconds local_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
			long long count = local_duration.count();
			nanoseconds += count;

            const Cost cost = cost_function.calculateCost(scene.getSchedules()[0]);
            costs.push_back(cost.getValue());

            std::cout << "#" << std::flush;
		}

		std::cout << std::endl;

        const float mean_cost = std::accumulate(costs.begin(), costs.end(), 0.f) / costs.size();
        deviations.push_back(mean_cost / optimal_value);
        const float mean_deviation = (mean_cost - optimal_value) / optimal_value;
        result.kpi.emplace(COST_KPI_NAME, std::to_string(mean_cost) + " (" + std::to_string(mean_deviation * 100) + "%) ");

        const float sq_sum = std::inner_product(costs.begin(), costs.end(), costs.begin(), 0.f);
        const float stdev2 = sq_sum / costs.size() - mean_cost * mean_cost;
        const float stdev = stdev2 > 0.f ? std::sqrt(stdev2) : 0.f;
        const float variation = stdev / mean_cost * 100.f;
        result.kpi.emplace(VARIATION_KPI_NAME, std::to_string(variation));

        result.kpi.emplace(AVERAGE_TIME_KPI_NAME, std::to_string(nanoseconds / (number_of_iterations * 1000000.0f)));

        total_cost += mean_cost;
        total_time += nanoseconds / (number_of_iterations * 1000000.0f);

		publisher.addResult(result);
	}

	float total_cost;
	std::vector<float> deviations;
	float total_time;

	TspLibRoutingService routing_service;
	BenchmarkPublisher& publisher;
	Engine engine;
	const std::vector<std::string>& datasets;
	ScheduleCostFunction& cost_function;
	
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
		TspLibSceneLoader scene_loader(engine);

		Scheduler::BenchmarkResult result;
		result.algorithm_name = getAlgorithmName();

		uint32_t optimal_value;

		result.dataset_name = datasets[id];

		scene_loader.loadScene(std::string(TSPLIB_BENCHMARK_DATA_ROOT) + "/" + datasets[id] + ".bin", routing_service, TspLibSceneLoader::Format::BINARY, optimal_value);

		result.kpi.emplace(COST_KPI_NAME, std::to_string(optimal_value));

		total_cost += optimal_value;

		publisher.addResult(result);
	}

	virtual TSPSolver& createTSPSolver() override
	{
		return engine.getAlgorithmsManager().createAlgorithm<TransparentTSPSolver>();
	}

	virtual const char* getAlgorithmName() override
	{
		return "**Optimal**";
	}
};

class Greedy_TspLibInstance : public TspLibTestInstance
{
public:
	Greedy_TspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TspLibTestInstance(datasets, publisher)
	{
	}

	virtual TSPSolver& createTSPSolver() override
	{
		GreedyTSPSolver& tsp_solver = engine.getAlgorithmsManager().createAlgorithm<GreedyTSPSolver>();
		tsp_solver.setRoutingService(routing_service);
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

	virtual TSPSolver& createTSPSolver() override
	{
		ChainTSPSolver& tsp_solver = engine.getAlgorithmsManager().createAlgorithm<ChainTSPSolver>();
		GreedyTSPSolver& greedy_solver = engine.getAlgorithmsManager().createAlgorithm<GreedyTSPSolver>();
		greedy_solver.setRoutingService(routing_service);

		SimpleTwoOptTSPSolver& two_opt_solver = engine.getAlgorithmsManager().createAlgorithm<SimpleTwoOptTSPSolver>();
		two_opt_solver.setScheduleCostFunction(cost_function);

		tsp_solver.addTSPSolver(greedy_solver);
		tsp_solver.addTSPSolver(two_opt_solver);
		return tsp_solver;
	}

	virtual const char* getAlgorithmName() override
	{
		return "Greedy >> 2-Opt";
	}
};

class Greedy_HybridOpt_TspLibInstance : public TspLibTestInstance
{
public:
    Greedy_HybridOpt_TspLibInstance (const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
        : TspLibTestInstance(datasets, publisher)
    {
    }

    virtual TSPSolver& createTSPSolver() override
    {
        ChainTSPSolver& tsp_solver = engine.getAlgorithmsManager().createAlgorithm<ChainTSPSolver>();
        GreedyTSPSolver& greedy_solver = engine.getAlgorithmsManager().createAlgorithm<GreedyTSPSolver>();
        greedy_solver.setRoutingService(routing_service);

        HybridOptTSPSolver& hybrid_opt_solver = engine.getAlgorithmsManager().createAlgorithm<HybridOptTSPSolver>();
        hybrid_opt_solver.setScheduleCostFunction(cost_function);

        tsp_solver.addTSPSolver(greedy_solver);
        tsp_solver.addTSPSolver(hybrid_opt_solver);
        return tsp_solver;
    }

    virtual const char* getAlgorithmName() override
    {
        return "Greedy >> Hybrid-Opt";
    }
};

class MTASA_FastTspLibInstance : public TspLibTestInstance
{
public:
    MTASA_FastTspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
        : TspLibTestInstance(datasets, publisher)
    {
        temperature_scheduler.reset(new FastTemperatureScheduler(120, std::log(std::pow(10,-10)), 1000, 0.25f));
    }

    virtual TSPSolver& createTSPSolver() override
    {
        ChainTSPSolver& tsp_solver = engine.getAlgorithmsManager().createAlgorithm<ChainTSPSolver>();
        GreedyTSPSolver& greedy_solver = engine.getAlgorithmsManager().createAlgorithm<GreedyTSPSolver>();
        greedy_solver.setRoutingService(routing_service);
        tsp_solver.addTSPSolver(greedy_solver);

        MultiAgentSimulatedAnnealingTSPSolver& sa_solver = engine.getAlgorithmsManager().createAlgorithm<MultiAgentSimulatedAnnealingTSPSolver>();
        sa_solver.setScheduleCostFunction(cost_function);
        sa_solver.setTemperatureScheduler(*temperature_scheduler);
        sa_solver.setMarkovChainLengthScale(1.f);
        sa_solver.setPopulationScale(2);
        sa_solver.setThreadsNumber(4);
        sa_solver.setMutations({
                                   SolutionGenerator::MutationType::BlockReverse,
                                   SolutionGenerator::MutationType::VertexInsert,
                                   SolutionGenerator::MutationType::BlockInsert
                                });
        sa_solver.addTSPSolver(engine.getAlgorithmsManager().createAlgorithm<ScrambleTSPSolver>());
        tsp_solver.addTSPSolver(sa_solver);
        return tsp_solver;
    }

    virtual const char* getAlgorithmName() override
    {
        return "MTASA_Fast";
    }
private:
    std::unique_ptr<TemperatureScheduler> temperature_scheduler;
};

class MTASA_NormalTspLibInstance : public TspLibTestInstance
{
public:
    MTASA_NormalTspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
        : TspLibTestInstance(datasets, publisher)
    {
        temperature_scheduler.reset(new ListTemperatureScheduler(120, std::log(std::pow(10,-10)), 1000));
    }

    virtual TSPSolver& createTSPSolver() override
    {
        ChainTSPSolver& tsp_solver = engine.getAlgorithmsManager().createAlgorithm<ChainTSPSolver>();
        GreedyTSPSolver& greedy_solver = engine.getAlgorithmsManager().createAlgorithm<GreedyTSPSolver>();
        greedy_solver.setRoutingService(routing_service);
        tsp_solver.addTSPSolver(greedy_solver);

        MultiAgentSimulatedAnnealingTSPSolver& sa_solver = engine.getAlgorithmsManager().createAlgorithm<MultiAgentSimulatedAnnealingTSPSolver>();
        sa_solver.setScheduleCostFunction(cost_function);
        sa_solver.setTemperatureScheduler(*temperature_scheduler);
        sa_solver.setMarkovChainLengthScale(1.f);
        sa_solver.setPopulationScale(2);
        sa_solver.setThreadsNumber(4);
        sa_solver.setMutations({
                                   SolutionGenerator::MutationType::BlockReverse,
                                   SolutionGenerator::MutationType::VertexInsert,
                                   SolutionGenerator::MutationType::BlockInsert
                                });
        sa_solver.addTSPSolver(engine.getAlgorithmsManager().createAlgorithm<ScrambleTSPSolver>());
        tsp_solver.addTSPSolver(sa_solver);
        return tsp_solver;
    }

    virtual const char* getAlgorithmName() override
    {
        return "MTASA_Normal";
    }
private:
    std::unique_ptr<TemperatureScheduler> temperature_scheduler;
};

class MTASA_SlowTspLibInstance : public TspLibTestInstance
{
public:
    MTASA_SlowTspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
        : TspLibTestInstance(datasets, publisher)
    {
        temperature_scheduler.reset(new SlowTemperatureScheduler(120, std::log(std::pow(10,-10)), 10000, 0.25f));
    }

    virtual TSPSolver& createTSPSolver() override
    {
        ChainTSPSolver& tsp_solver = engine.getAlgorithmsManager().createAlgorithm<ChainTSPSolver>();
        GreedyTSPSolver& greedy_solver = engine.getAlgorithmsManager().createAlgorithm<GreedyTSPSolver>();
        greedy_solver.setRoutingService(routing_service);
        tsp_solver.addTSPSolver(greedy_solver);

        MultiAgentSimulatedAnnealingTSPSolver& sa_solver = engine.getAlgorithmsManager().createAlgorithm<MultiAgentSimulatedAnnealingTSPSolver>();
        sa_solver.setScheduleCostFunction(cost_function);
        sa_solver.setTemperatureScheduler(*temperature_scheduler);
        sa_solver.setMarkovChainLengthScale(1.f);
        sa_solver.setPopulationScale(2);
        sa_solver.setThreadsNumber(4);
        sa_solver.setMutations({
                                   SolutionGenerator::MutationType::BlockReverse,
                                   SolutionGenerator::MutationType::VertexInsert,
                                   SolutionGenerator::MutationType::BlockInsert
                                });
        sa_solver.addTSPSolver(engine.getAlgorithmsManager().createAlgorithm<ScrambleTSPSolver>());
        tsp_solver.addTSPSolver(sa_solver);
        return tsp_solver;
    }

    virtual const char* getAlgorithmName() override
    {
        return "MTASA_Slow";
    }
private:
    std::unique_ptr<TemperatureScheduler> temperature_scheduler;
};

class OneRelocate_TspLibInstance : public TspLibTestInstance
{
public:
	OneRelocate_TspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TspLibTestInstance(datasets, publisher)
	{
	}

	virtual TSPSolver& createTSPSolver() override
	{
		OneRelocateTSPSolver& tsp_solver = engine.getAlgorithmsManager().createAlgorithm<OneRelocateTSPSolver>();
		tsp_solver.setScheduleCostFunction(cost_function);
		
		return tsp_solver;
	}

	virtual const char* getAlgorithmName() override
	{
		return "1-Relocate";
	}
};

class Greedy_TwoOpt_OneRelocate_TspLibInstance : public TspLibTestInstance
{
public:
	Greedy_TwoOpt_OneRelocate_TspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TspLibTestInstance(datasets, publisher)
	{
	}

	virtual TSPSolver& createTSPSolver() override
	{
		ChainTSPSolver& tsp_solver = engine.getAlgorithmsManager().createAlgorithm<ChainTSPSolver>();

		GreedyTSPSolver& greedy_solver = engine.getAlgorithmsManager().createAlgorithm<GreedyTSPSolver>();
		greedy_solver.setRoutingService(routing_service);

		OneRelocateTSPSolver& onerel_solver = engine.getAlgorithmsManager().createAlgorithm<OneRelocateTSPSolver>();
		onerel_solver.setScheduleCostFunction(cost_function);

		SimpleTwoOptTSPSolver& two_opt_solver = engine.getAlgorithmsManager().createAlgorithm<SimpleTwoOptTSPSolver>();
		two_opt_solver.setScheduleCostFunction(cost_function);

		tsp_solver.addTSPSolver(greedy_solver);
		tsp_solver.addTSPSolver(two_opt_solver);
		tsp_solver.addTSPSolver(onerel_solver);
		
		return tsp_solver;
	}

	virtual const char* getAlgorithmName() override
	{
		return "Greedy >> 2-Opt >> 1-Relocate";
	}
};

class SuIntTspLibInstance : public TspLibTestInstance
{
public:
	SuIntTspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TspLibTestInstance(datasets, publisher)
	{
	}

	virtual TSPSolver& createTSPSolver() override
	{
		ChainTSPSolver &tsp_solver = engine.getAlgorithmsManager().createAlgorithm<ChainTSPSolver>();

		GreedyTSPSolver &greedy_solver = engine.getAlgorithmsManager().createAlgorithm<GreedyTSPSolver>();
		greedy_solver.setRoutingService(routing_service);

		SuIntTSPSolver& suint_solver = engine.getAlgorithmsManager().createAlgorithm<SuIntTSPSolver>();
		suint_solver.setCostFunction(cost_function);
		suint_solver.setRoutingService(routing_service);
		suint_solver.setEdgeSuggestor(EdgeSuggestorType::BETTER_EDGE);
		suint_solver.addEdgeIntroducer(EdgeIntroducerType::REVERSE);
		suint_solver.addEdgeIntroducer(EdgeIntroducerType::DIRECT);
		suint_solver.addEdgeIntroducer(EdgeIntroducerType::CIRCULAR);

		tsp_solver.addTSPSolver(greedy_solver);
		tsp_solver.addTSPSolver(suint_solver);

		return tsp_solver;
	}

	virtual const char* getAlgorithmName() override
	{
		return "Greedy >> SuInt";
	}
};

class DoubleSuIntTspLibInstance : public TspLibTestInstance
{
public:
	DoubleSuIntTspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TspLibTestInstance(datasets, publisher)
	{
	}

	virtual TSPSolver& createTSPSolver() override
	{
		ChainTSPSolver& main_solver = engine.getAlgorithmsManager().createAlgorithm<ChainTSPSolver>();

		GreedyTSPSolver& greedy_solver = engine.getAlgorithmsManager().createAlgorithm<GreedyTSPSolver>();
		greedy_solver.setRoutingService(routing_service);

		SuIntTSPSolver& suint_solver = engine.getAlgorithmsManager().createAlgorithm<SuIntTSPSolver>();
		suint_solver.setCostFunction(cost_function);
		suint_solver.setRoutingService(routing_service);
		suint_solver.setEdgeSuggestor(EdgeSuggestorType::BETTER_EDGE);
		suint_solver.addEdgeIntroducer(EdgeIntroducerType::REVERSE);
		suint_solver.addEdgeIntroducer(EdgeIntroducerType::DIRECT);
		suint_solver.addEdgeIntroducer(EdgeIntroducerType::CIRCULAR);
		
		ReverseTSPSolver& reverse_solver = engine.getAlgorithmsManager().createAlgorithm<ReverseTSPSolver>();
		ChainTSPSolver& second_suint_solver = engine.getAlgorithmsManager().createAlgorithm<ChainTSPSolver>();
		second_suint_solver.addTSPSolver(reverse_solver);
		second_suint_solver.addTSPSolver(suint_solver);

		TransparentTSPSolver& transparent_solver = engine.getAlgorithmsManager().createAlgorithm<TransparentTSPSolver>();
		BestOfTSPSolver& best_of_solver = engine.getAlgorithmsManager().createAlgorithm<BestOfTSPSolver>();
		best_of_solver.addTSPSolver(transparent_solver);
		best_of_solver.addTSPSolver(second_suint_solver);
		best_of_solver.setScheduleCostFunction(cost_function);
		
		main_solver.addTSPSolver(greedy_solver);
		main_solver.addTSPSolver(suint_solver);
		main_solver.addTSPSolver(best_of_solver);

		return main_solver;
	}

	virtual const char* getAlgorithmName() override
	{
		return "Greedy >> SuInt >> Reverse >> SuInt";
	}
};

int main(int argc, char **argv)
{
	LoggingManager::configure("logging.cfg");

	std::unique_ptr<Scheduler::BenchmarkPublisher> publisher;
	if (argc > 1)
	{
		publisher.reset(new MarkdownBenchmarkPublisher(argv[1]));
	}
	else
	{
		publisher.reset(new StdoutBenchmarkPublisher());
	}

    auto datasets = {light_datasets/*, medium_datasets*/};
    for (const auto &dataset : datasets) {
		
        {
            Optimal_TspLibInstance test(dataset, *publisher);
            test.run();
        }

        {
            Greedy_TspLibInstance test(dataset, *publisher);
            test.run();
        }

        {
            Greedy_2Opt_TspLibInstance test(dataset, *publisher);
            test.run();
        }

        {
            Greedy_TwoOpt_OneRelocate_TspLibInstance test(dataset, *publisher);
            test.run();
        }

        {
            Greedy_HybridOpt_TspLibInstance test(dataset, *publisher);
            test.run();
        }

        {
            MTASA_FastTspLibInstance test(dataset, *publisher);
            test.run();
        }

        {
            MTASA_NormalTspLibInstance test(dataset, *publisher);
            test.run();
        }

        {
            MTASA_SlowTspLibInstance test(dataset, *publisher);
            test.run();
        }
		
        {
			SuIntTspLibInstance test(dataset, *publisher);
			test.run();
        }
        
        {
			DoubleSuIntTspLibInstance test(dataset, *publisher);
			test.run();
        }
    }
	publisher->publish();
}
