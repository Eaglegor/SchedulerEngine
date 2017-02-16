#include <numeric>


#include <Services/Routing/TspLibRoutingService/TspLibRoutingService.h>
#include <Persistence/SceneLoaders/TspLibSceneLoader/TspLibSceneLoader.h>
#include <chrono>
#include <boost/program_options.hpp>
#include <cmath>
#include <iostream>
#include <sstream>

#include "../Publishers/StdoutBenchmarkPublisher.h"
#include "../Publishers/MarkdownBenchmarkPublisher.h"
#include <Engine/Core/Engine.h>
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
static size_t number_of_iterations = 10;

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

class SimulatedAnnealingTspLibInstance : public TspLibTestInstance
{
public:
    struct Options
    {
        struct TemperatureSchedulerOptions
        {
            enum class Type
            {
                ListNormal,
                ListFast,
                ListSlow,
                Default
            } type;
            std::size_t list_size;
            std::size_t max_iterations;
            float initial_probability;
            float end_probability;
            float ratio;
            TemperatureSchedulerOptions() :
            type(Type::ListNormal),
            list_size(120),
            max_iterations(1000),
            initial_probability(std::log(std::pow(10,-10))),
            end_probability(std::log(std::pow(10,-50))),
            ratio(0.25f) {}
        } temperature_scheduler_options;
        float markov_chain_length_scale;
        std::size_t population_size;
        std::size_t threads_number;
        Options() : markov_chain_length_scale(1.f), population_size(8), threads_number(1) {}
    };
    
    SimulatedAnnealingTspLibInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
        : TspLibTestInstance(datasets, publisher) {}
    
    void setOptions (const Options & options)
    {
        this->options = options;
    }

    virtual TSPSolver& createTSPSolver() override
    {
        SimulatedAnnealingTSPSolver& sa_solver = engine.getAlgorithmsManager().createAlgorithm<SimulatedAnnealingTSPSolver>();
        setTemperatureScheduler(options.temperature_scheduler_options);
        sa_solver.setTemperatureScheduler(*temperature_scheduler);
        sa_solver.setScheduleCostFunction(cost_function);
        sa_solver.setMarkovChainLengthScale(options.markov_chain_length_scale);
        sa_solver.setPopulationSize(options.population_size);
        sa_solver.setThreadsNumber(options.threads_number);
        sa_solver.setMutations({
                                   SolutionGenerator::MutationType::BlockReverse,
                                   SolutionGenerator::MutationType::VertexInsert,
                                   SolutionGenerator::MutationType::BlockInsert
                                });
        
        return sa_solver;
    }

    virtual const char* getAlgorithmName() override
    {
        return "SimulatedAnnealing";
    }
private:
    void setTemperatureScheduler(const Options::TemperatureSchedulerOptions & options)
    {
        switch (options.type) {
            case Options::TemperatureSchedulerOptions::Type::ListFast:
                temperature_scheduler.reset(new FastTemperatureScheduler(options.list_size,
                                                                         options.initial_probability,
                                                                         options.max_iterations,
                                                                         options.ratio));
                break;
            case Options::TemperatureSchedulerOptions::Type::ListSlow:
                temperature_scheduler.reset(new SlowTemperatureScheduler(options.list_size,
                                                                         options.initial_probability,
                                                                         options.max_iterations,
                                                                         options.ratio));
                break;
            case Options::TemperatureSchedulerOptions::Type::ListNormal:
                temperature_scheduler.reset(new ListTemperatureScheduler(options.list_size,
                                                                         options.initial_probability,
                                                                         options.max_iterations));
            break;
            case Options::TemperatureSchedulerOptions::Type::Default:
            default:
                temperature_scheduler.reset(new DefaultTemperatureScheduler(options.initial_probability,
                                                                            options.end_probability,
                                                                            options.max_iterations));
                break;
        }
    }
    std::unique_ptr<TemperatureScheduler> temperature_scheduler;
    Options options;
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

static std::string light_datasets_name = "light";
static std::string medium_datasets_name = "medium";
static std::string heavy_datasets_name = "heavy";
static std::string huge_datasets_name = "huge";

static std::string optimal_solver_name = "optimal";
static std::string greedy_solver_name = "greedy";
static std::string two_opt_solver_name = "two-opt";
static std::string one_relocate_solver_name = "one-relocate";
static std::string hybrid_solver_name = "hybrid-opt";
static std::string sa_solver_name = "sa";
static std::string suint_solver_name = "suint";
static std::string double_suint_solver_name = "suint-double";

std::map<std::string, std::vector<std::string>> datasets_map = {
    {light_datasets_name, std::ref(light_datasets)},
    {medium_datasets_name, std::ref(medium_datasets)},
    {heavy_datasets_name, std::ref(heavy_datasets)},
    {huge_datasets_name, std::ref(huge_datasets)}};

struct CmdLineOptions
{
    std::vector<std::string> dataset_names;
    std::vector<std::string> solver_names;
    std::string output_file_name;
    std::size_t number_of_iterations;
    SimulatedAnnealingTspLibInstance::Options sa_options;
};

bool parseCommandLine (int argc, char **argv, CmdLineOptions & cmd_line_options)
{
    static std::map<std::string, SimulatedAnnealingTspLibInstance::Options::TemperatureSchedulerOptions::Type> sa_temperature_scheduler_map = {
        {"list-normal", SimulatedAnnealingTspLibInstance::Options::TemperatureSchedulerOptions::Type::ListNormal},
        {"list-fast", SimulatedAnnealingTspLibInstance::Options::TemperatureSchedulerOptions::Type::ListFast},
        {"list-slow", SimulatedAnnealingTspLibInstance::Options::TemperatureSchedulerOptions::Type::ListSlow},
        {"default", SimulatedAnnealingTspLibInstance::Options::TemperatureSchedulerOptions::Type::Default}
    };

    std::string sa_temperature_scheduler_name;
    boost::program_options::options_description options_description("Allowed options");
    options_description.add_options()
    ("help", "produce help message")
    ("datasets-name,D",
     boost::program_options::value<std::vector<std::string>>(&cmd_line_options.dataset_names),
     "set dataset name")
    ("solver-name,S",
     boost::program_options::value<std::vector<std::string>>(&cmd_line_options.solver_names),
     "set solver name")
    ("iterations-number,N",
     boost::program_options::value<std::size_t>(&cmd_line_options.number_of_iterations)->default_value(10),
     "set number of iterations")
    ("output-file-name,O",
     boost::program_options::value<std::string>(&cmd_line_options.output_file_name),
     "output file name")
    ("sa-population-size",
     boost::program_options::value<std::size_t>(&cmd_line_options.sa_options.population_size)->default_value(cmd_line_options.sa_options.population_size),
     "simulated annealing population size")
    ("sa-threads-number",
     boost::program_options::value<std::size_t>(&cmd_line_options.sa_options.threads_number)->default_value(cmd_line_options.sa_options.threads_number),
     "simulated annealing threads number")
    ("sa-markov-chain-length-scale",
     boost::program_options::value<float>(&cmd_line_options.sa_options.markov_chain_length_scale)->default_value(cmd_line_options.sa_options.markov_chain_length_scale),
     "simulated annealing markov chain lenght scale")
    ("sa-temperature-scheduler-name",
     boost::program_options::value<std::string>(&sa_temperature_scheduler_name)->default_value("list-normal"),
     "simulated annealing temperature scheduler name")
    ("sa-list-temperature-scheduler-size",
     boost::program_options::value<std::size_t>(&cmd_line_options.sa_options.temperature_scheduler_options.list_size)->default_value(cmd_line_options.sa_options.temperature_scheduler_options.list_size),
     "simulated annealing list temperature scheduler size")
    ("sa-temperature-scheduler-limit",
     boost::program_options::value<std::size_t>(&cmd_line_options.sa_options.temperature_scheduler_options.max_iterations)->default_value(cmd_line_options.sa_options.temperature_scheduler_options.max_iterations),
     "simulated annealing temperature scheduler iterations limit")
    ("sa-list-temperature-scheduler-ratio",
     boost::program_options::value<float>(&cmd_line_options.sa_options.temperature_scheduler_options.ratio)->default_value(cmd_line_options.sa_options.temperature_scheduler_options.ratio),
     "simulated annealing list temperature scheduler ratio")
    ("sa-temperature-scheduler-initial-probability",
     boost::program_options::value<float>(&cmd_line_options.sa_options.temperature_scheduler_options.initial_probability)->default_value(cmd_line_options.sa_options.temperature_scheduler_options.initial_probability),
     "simulated annealing temperature scheduler natural log initial probability")
    ("sa-temperature-scheduler-end-probability",
     boost::program_options::value<float>(&cmd_line_options.sa_options.temperature_scheduler_options.end_probability)->default_value(cmd_line_options.sa_options.temperature_scheduler_options.end_probability),
     "simulated annealing temperature scheduler natural log end probability");

    boost::program_options::positional_options_description p;
    p.add("output-file-name", -1);
    
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(options_description).positional(p).run(), vm);
    boost::program_options::notify(vm);
    
    if (vm.count("help")) {
        std::cout << options_description << std::endl;
        return false;
    }
    
    if (cmd_line_options.dataset_names.empty()) {
        cmd_line_options.dataset_names.push_back(light_datasets_name);
    }

    if (cmd_line_options.solver_names.empty()) {
        cmd_line_options.solver_names.insert(cmd_line_options.solver_names.end(), { optimal_solver_name,
                                                                                    greedy_solver_name,
                                                                                    two_opt_solver_name,
                                                                                    one_relocate_solver_name,
                                                                                    hybrid_solver_name,
                                                                                    sa_solver_name,
                                                                                    suint_solver_name,
                                                                                    double_suint_solver_name });
    }
    
    cmd_line_options.sa_options.temperature_scheduler_options.type = sa_temperature_scheduler_map.at(sa_temperature_scheduler_name);

    return true;
}

int main(int argc, char **argv)
{
    LoggingManager::configure("logging.cfg");
    
    CmdLineOptions cmd_line_options;
    if (!parseCommandLine(argc, argv, cmd_line_options)) {
        return 1;
    }
    
    number_of_iterations = cmd_line_options.number_of_iterations;
    
	std::unique_ptr<Scheduler::BenchmarkPublisher> publisher;
	if (!cmd_line_options.output_file_name.empty())
	{
		publisher.reset(new MarkdownBenchmarkPublisher(cmd_line_options.output_file_name));
	}
	else
	{
		publisher.reset(new StdoutBenchmarkPublisher());
	}

    for (const auto &dataset_name : cmd_line_options.dataset_names) {
        const auto & dataset = datasets_map.at(dataset_name);
        for (const auto & solver_name : cmd_line_options.solver_names) {
            if (solver_name == optimal_solver_name) {
                Optimal_TspLibInstance test(dataset, *publisher);
                test.run();
            }
            
            if (solver_name == greedy_solver_name) {
                Greedy_TspLibInstance test(dataset, *publisher);
                test.run();
            }
            
            if (solver_name == two_opt_solver_name) {
                Greedy_2Opt_TspLibInstance test(dataset, *publisher);
                test.run();
            }
            
            if (solver_name == one_relocate_solver_name) {
                Greedy_TwoOpt_OneRelocate_TspLibInstance test(dataset, *publisher);
                test.run();
            }
            
            if (solver_name == hybrid_solver_name) {
                Greedy_HybridOpt_TspLibInstance test(dataset, *publisher);
                test.run();
            }
            
            if (solver_name == sa_solver_name) {
                SimulatedAnnealingTspLibInstance test(dataset, *publisher);
                test.setOptions(cmd_line_options.sa_options);
                test.run();
            }
            
            if (solver_name == suint_solver_name) {
                SuIntTspLibInstance test(dataset, *publisher);
                test.run();
            }
            
            if (solver_name == double_suint_solver_name) {
                DoubleSuIntTspLibInstance test(dataset, *publisher);
                test.run();
            }
        }
    }
	publisher->publish();
}
