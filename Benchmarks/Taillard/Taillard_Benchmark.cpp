#include <Services/Routing/TaillardRoutingService/TaillardRoutingService.h>
#include <Persistence/SceneLoaders/TaillardSceneLoader/TaillardSceneLoader.h>
#include <chrono>

#include <cmath>
#include <iostream>

#include "../Publishers/StdoutBenchmarkPublisher.h"
#include "../Publishers/MarkdownBenchmarkPublisher.h"
#include <Engine/Algorithms/VRPSolvers/Utilitary/Transparent/TransparentVRPSolver.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/Chain/ChainVRPSolver.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/TSPOnly/TSPOnlyVRPSolver.h>
#include <Engine/Algorithms/VRPSolvers/Sweep/SweepVRPSolver.h>
#include <Engine/Engine/Engine.h>
#include <Engine/CostFunctions/TotalDistance/TotalDistanceSceneCostFunction.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/Algorithms/TSPSolvers/SimulatedAnnealing/SimulatedAnnealingTSPSolver.h>
#include <Engine/CostFunctions/TotalDistance/TotalDistanceScheduleCostFunction.h>
#include <Engine/Algorithms/TSPSolvers/SimulatedAnnealing/ListTemperatureScheduler.h>
#include <Engine/Algorithms/TSPSolvers/Utilitary/Chain/ChainTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/Greedy/GreedyTSPSolver.h>
#include <Engine/Algorithms/TSPSolvers/SimpleTwoOpt/SimpleTwoOptTSPSolver.h>

std::vector<std::string> datasets
{
	"tai100a",
	"tai100b",
	"tai100c",
	"tai100d",
	"tai150a",
	"tai150b",
	"tai150c",
	"tai150d",
	"tai385",
	"tai75a",
	"tai75b",
	"tai75c",
	"tai75d"
};

std::unordered_map<std::string, float> optimal_costs
{
	{ "tai100a", 2041.336464},
	{ "tai100b", 1940.608},
	{ "tai100c", 1406.20},
	{ "tai100d", 1537.064},
	{ "tai150a", 3055.23},
	{ "tai150b", 2656.47},
	{ "tai150c", 2341.84},
	{ "tai150d", 2645.39},
	{ "tai385", 24435.462950 },
	{ "tai75a", 1618.352},
	{ "tai75b", 1344.617 },
	{ "tai75c", 1291.008 },
	{ "tai75d", 1365.421 }
};

std::unordered_map<std::string, long> optimal_used_vehicles_values
{
	{ "tai100a", 11 },
	{ "tai100b", 11 },
	{ "tai100c", 11 },
	{ "tai100d", 11 },
	{ "tai150a", 0 },
	{ "tai150b", 0 },
	{ "tai150c", 0 },
	{ "tai150d", 0 },
	{ "tai385", 47 },
	{ "tai75a", 10 },
	{ "tai75b", 10 },
	{ "tai75c", 9 },
	{ "tai75d", 9 }
};

const char* COST_KPI_NAME = "Cost";
const char* USED_VEHICLES_KPI_NAME = "Used vehicles";
const char* AVERAGE_TIME_KPI_NAME = "Average time (ms)";
const char* SCHEDULED_ORDERS_KPI_NAME = "Scheduled orders";

using namespace Scheduler;

class TaillardTestInstance
{
public:
	virtual ~TaillardTestInstance(){}

	TaillardTestInstance(const std::vector<std::string> &datasets, BenchmarkPublisher& publisher):
		publisher(publisher),
		datasets(datasets)
	{
		engine.reset(new Engine());

		strategy = engine->getStrategiesManager()->createStrategy();
		cost_function = strategy->createSceneCostFunction<TotalDistanceSceneCostFunction>();
	}

	virtual VRPSolver* createVRPSolver(Strategy* strategy) = 0;
	virtual const char* getAlgorithmName() = 0;

	void run()
	{
		std::cout << "############# Testing solver: "  << getAlgorithmName() << " ####################" << std::endl;

		total_cost = 0;
		total_time = 0;
		total_used_vehicles = 0;

		for (size_t i = 0; i < datasets.size(); ++i)
		{
			runDataset(i);
		}

		BenchmarkResult result;
		result.algorithm_name = getAlgorithmName();
		result.dataset_name = "{Summary}";
		result.kpi.emplace(COST_KPI_NAME, std::to_string(total_cost));
		result.kpi.emplace(USED_VEHICLES_KPI_NAME, std::to_string(total_used_vehicles));
		if(total_time > FLOAT_EPSILON) result.kpi.emplace(AVERAGE_TIME_KPI_NAME, std::to_string(total_time));
		publisher.addResult(result);
	}

protected:

	virtual void runDataset(size_t id)
	{
		TaillardSceneLoader scene_loader(engine->getSceneManager(), &routing_service);

		Scheduler::BenchmarkResult result;
		result.algorithm_name = getAlgorithmName();

		VRPSolver* solver = createVRPSolver(strategy);

		Cost cost;

		float optimal_value = optimal_costs.at(datasets[id]);
		long optimal_used_vehicles = optimal_used_vehicles_values.at(datasets[id]);

        std::cout << "Running " << id + 1 << "/" << datasets.size() << ": " << datasets[id] << " " << std::flush;
		result.dataset_name = datasets[id];

		long long nanoseconds = 0;

		long used_vehicles = 0;
		size_t scheduled_orders = 0;
		size_t total_orders = 0;

		for (size_t i = 0; i < 10; ++i)
		{
			Scene* scene = scene_loader.loadScene(std::string(TAILLARD_BENCHMARK_DATA_ROOT) + "/" + datasets[id] + ".dat");

			total_orders = scene->getContext().getOrders().size();

			std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
			solver->optimize(scene);
			std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

			std::chrono::nanoseconds local_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
			long long count = local_duration.count();
			nanoseconds += count;

			cost = cost_function->calculateCost(scene);
			
			used_vehicles = 0;
			for(Schedule* schedule : scene->getSchedules())
			{
				if(!schedule->getRuns().empty())
				{
					++used_vehicles;
				}
			}
			
			scheduled_orders = 0;
			for(Order* order : scene->getContext().getOrders())
			{
				Operation* operation = order->getWorkOperation();

				size_t occurances = 0;

				for(Schedule* schedule : scene->getSchedules())
				{
					for(Run* run : schedule->getRuns())
					{
						for(WorkStop* stop : run->getWorkStops())
						{
							if (stop->getOperation() == operation) ++occurances;
						}
					}
				}

				scheduled_orders += occurances;
			}

            std::cout << "#" << std::flush;
		}
		

		std::cout << std::endl;

		float deviation = (cost.getValue() - optimal_value) / static_cast<float>(optimal_value);

		result.kpi.emplace(SCHEDULED_ORDERS_KPI_NAME, std::to_string(scheduled_orders) + " / " + std::to_string(total_orders));
		result.kpi.emplace(COST_KPI_NAME, std::to_string(cost.getValue()) + " (" + std::to_string(deviation * 100) + "%) ");
		result.kpi.emplace(AVERAGE_TIME_KPI_NAME, std::to_string(nanoseconds / 10000000.0f));
		result.kpi.emplace(USED_VEHICLES_KPI_NAME, std::to_string(used_vehicles) + (used_vehicles != optimal_used_vehicles ? ( " ("+ (used_vehicles > optimal_used_vehicles ? std::string("+") : std::string("")) + std::to_string(used_vehicles - optimal_used_vehicles) +")" ) : ""));

		total_cost += cost.getValue();
		total_time += nanoseconds / 10000000.0f;
		total_used_vehicles += used_vehicles;

		publisher.addResult(result);
	}

	float total_cost;
	size_t total_used_vehicles;
	float total_time;

	Scheduler::TaillardRoutingService routing_service;
	Scheduler::BenchmarkPublisher& publisher;
	std::unique_ptr<Scheduler::Engine> engine;
	Scheduler::Strategy* strategy;
	const std::vector<std::string>& datasets;
	SceneCostFunction* cost_function;
	
};

class Optimal_TaillardTestInstance : public TaillardTestInstance
{
public:
	Optimal_TaillardTestInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TaillardTestInstance(datasets, publisher)
	{
	}

	virtual void runDataset(size_t id) override
	{
		TaillardSceneLoader scene_loader(engine->getSceneManager(), &routing_service);

		Scheduler::BenchmarkResult result;
		result.algorithm_name = getAlgorithmName();

		float optimal_value = optimal_costs.at(datasets[id]);
		long optimal_used_vehicles = optimal_used_vehicles_values.at(datasets[id]);

		result.dataset_name = datasets[id];

		Scene* scene = scene_loader.loadScene(std::string(TAILLARD_BENCHMARK_DATA_ROOT) + "/" + datasets[id] + ".dat");

		result.kpi.emplace(USED_VEHICLES_KPI_NAME, std::to_string(optimal_used_vehicles));
		result.kpi.emplace(COST_KPI_NAME, std::to_string(optimal_value));

		total_cost += optimal_value;
		total_used_vehicles += optimal_used_vehicles;

		publisher.addResult(result);
	}

	virtual VRPSolver* createVRPSolver(Strategy* strategy) override
	{
		return nullptr;
	}

	virtual const char* getAlgorithmName() override
	{
		return "**Optimal**";
	}
};

class Transparent_TaillardTestInstance : public TaillardTestInstance
{
public:
	Transparent_TaillardTestInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
		: TaillardTestInstance(datasets, publisher)
	{
	}

	virtual VRPSolver* createVRPSolver(Strategy* strategy) override
	{
		TransparentVRPSolver *vrp_solver = strategy->createVRPSolver<TransparentVRPSolver>();
		return vrp_solver;
	}

	virtual const char* getAlgorithmName() override
	{
		return "Transparent";
	}
};

class Sweep_TaillardTestInstance : public TaillardTestInstance
{
public:
    Sweep_TaillardTestInstance(const std::vector<std::string>& datasets, BenchmarkPublisher& publisher)
        : TaillardTestInstance(datasets, publisher)
    {
    }

    virtual VRPSolver* createVRPSolver(Strategy* strategy) override
    {
        ChainVRPSolver* chain_solver = strategy->createVRPSolver<ChainVRPSolver>();

        SweepVRPSolver* sweep_solver = strategy->createVRPSolver<SweepVRPSolver>();
        chain_solver->appendSolver(sweep_solver);

        TSPOnlyVRPSolver* tsponly_solver = strategy->createVRPSolver<TSPOnlyVRPSolver>(createTSPSolver(strategy));
        chain_solver->appendSolver(tsponly_solver);

        return chain_solver;
    }

    virtual const char* getAlgorithmName() override
    {
        return "Sweep";
    }
private:
    TSPSolver* createTSPSolverSA (Strategy* strategy)
    {
        SimulatedAnnealingTSPSolver* tsp_solver = strategy->createTSPSolver<SimulatedAnnealingTSPSolver>();
        tsp_solver->setScheduleCostFunction(strategy->createScheduleCostFunction<TotalDistanceScheduleCostFunction>());
        tsp_solver->setTemperatureScheduler(new ListTemperatureScheduler(120, std::log(std::pow(10, -3)), 1000));
        tsp_solver->setMarkovChainLengthScale(2.f);
        return tsp_solver;
    }

    TSPSolver* createTSPSolver2Opt (Strategy* strategy)
    {
        ChainTSPSolver* tsp_solver = strategy->createTSPSolver<ChainTSPSolver>();

        GreedyTSPSolver* greedy_solver = strategy->createTSPSolver<GreedyTSPSolver>();
        greedy_solver->setRoutingService(&routing_service);
        tsp_solver->addTSPSolver(greedy_solver);

        SimpleTwoOptTSPSolver* two_opt_solver = strategy->createTSPSolver<SimpleTwoOptTSPSolver>();
        two_opt_solver->setScheduleCostFunction(strategy->createScheduleCostFunction<TotalDistanceScheduleCostFunction>());
        tsp_solver->addTSPSolver(two_opt_solver);

        return tsp_solver;
    }

    TSPSolver* createTSPSolver (Strategy* strategy)
    {
        return createTSPSolver2Opt(strategy);
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
		Optimal_TaillardTestInstance test(datasets, *publisher);
		test.run();
	}

	{
		Transparent_TaillardTestInstance test(datasets, *publisher);
		test.run();
    }

    {
        Sweep_TaillardTestInstance test(datasets, *publisher);
        test.run();
    }
	
	publisher->publish();
}
