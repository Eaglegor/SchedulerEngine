#include "SimulatedAnnealingTSPSolver.h"
#include "InstanceBasedSolutionGenerator.h"
#include "TemperatureScheduler.h"
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/ScheduleVariant.h>
#include <Engine/SceneManager/Utils/SceneCloner.h>
#include <random>

namespace Scheduler
{
	SimulatedAnnealingTSPSolver::SimulatedAnnealingTSPSolver( )
	    : allowed_mutations({SolutionGenerator::MutationType::BlockInsert,
	                         SolutionGenerator::MutationType::BlockReverse,
	                         SolutionGenerator::MutationType::VertexInsert,
	                         SolutionGenerator::MutationType::VertexSwap}),
	      markov_chain_length_scale(1.f),
	      population_size(2),
	      threads_number(1),
	      multithreading_type(MultithreadingType::Auto),
	      logger(LoggingManager::getLogger("SimulatedAnnealingTSPSolver"))
	{
	}

	void SimulatedAnnealingTSPSolver::optimize(Schedule& schedule) const
	{
		TRACEABLE_SECTION(__optimize__, "optimize(Schedule&)", logger);

		if(!schedule_cost_function)
		{
			LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
			return;
		}

		if(!temperature_scheduler_template)
		{
			LOG_WARNING(logger, "Temperature scheduler is not set. Can't solve TSP.");
			return;
		}

		if(allowed_mutations.empty( ))
		{
			LOG_WARNING(logger, "Allowed mutations is not set. Can't solve TSP.");
			return;
		}

		for(Run& run : schedule.getRuns( ))
		{
			optimize(run);
		}
	}

	void SimulatedAnnealingTSPSolver::optimize(Run& run) const
	{
		TRACEABLE_SECTION(__optimize__, "optimize(Run&)", logger);

		if(!schedule_cost_function)
		{
			LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
			return;
		}

		if(!temperature_scheduler_template)
		{
			LOG_WARNING(logger, "Temperature scheduler is not set. Can't solve TSP.");
			return;
		}

		if(allowed_mutations.empty( ))
		{
			LOG_WARNING(logger, "Allowed mutations is not set. Can't solve TSP.");
			return;
		}

		if(run.getWorkStops( ).size( ) <= 1)
		{
			LOG_WARNING(logger, "Too small run size. Can't solve TSP.");
			return;
		}

		switch(multithreading_type)
		{
			case MultithreadingType::Independent:
				optimizeIndependent(run);
				break;
			case MultithreadingType::Iterative:
				optimizeIterative(run);
				break;
			case MultithreadingType::Auto:
			default:
				optimizeAuto(run);
				break;
		}
	}

	void SimulatedAnnealingTSPSolver::setScheduleCostFunction(const ScheduleCostFunction& cost_function)
	{
		this->schedule_cost_function = cost_function;
	}

	void SimulatedAnnealingTSPSolver::setTemperatureScheduler(const TemperatureScheduler& temperature_function)
	{
		this->temperature_scheduler_template = temperature_function;
	}

	void SimulatedAnnealingTSPSolver::setMarkovChainLengthScale(float markov_scale)
	{
		this->markov_chain_length_scale = markov_scale;
	}

	void SimulatedAnnealingTSPSolver::setMutations(std::initializer_list<SolutionGenerator::MutationType> mutations)
	{
		this->allowed_mutations.clear( );
		this->allowed_mutations.insert(mutations.begin( ), mutations.end( ));
	}

	std::size_t SimulatedAnnealingTSPSolver::markovChainLength(std::size_t stops_count) const
	{
		const size_t M = std::nearbyint(stops_count * markov_chain_length_scale);
		return std::max(std::size_t(1), M);
	}

	void SimulatedAnnealingTSPSolver::setPopulationSize(size_t population_size)
	{
		this->population_size = std::max(static_cast<std::size_t>(2), population_size);
	}

	void SimulatedAnnealingTSPSolver::setThreadsNumber(std::size_t threads_number)
	{
		this->threads_number = threads_number;
	}

	void SimulatedAnnealingTSPSolver::setMultithreadingType(MultithreadingType multithreading_type)
	{
		this->multithreading_type = multithreading_type;
	}

	bool SimulatedAnnealingTSPSolver::acceptance(Cost delta, float random, float temperature) const
	{
		const float normalized_value = std::exp(-delta.getValue( ) / temperature);
		return random <= normalized_value;
	}

	void SimulatedAnnealingTSPSolver::initialize(Run& run,
	                                             VectorSchedules& schedules,
	                                             VectorRuns& runs,
	                                             VectorCosts& costs,
	                                             VectorGenerators& generators,
	                                             VectorTemperatureSchedulers& temperature_schedulers) const
	{
		TRACEABLE_SECTION(__initialize__, "initialize", logger);

		auto run_iter             = run.getSchedule( ).findRun(run);
		const std::size_t run_idx = std::distance<Schedule::ConstRunIterator>(run.getSchedule( ).getRuns( ).begin( ), run_iter);

		const std::size_t N = population_size;
		for(std::size_t i = 0; i < N; ++i)
		{
			schedules.emplace_back(run.getSchedule( ));
			ScheduleVariant& temporary_schedule = schedules.back( );

			Run& temporary_run = temporary_schedule.getSchedule( )->getRuns( ).at(run_idx);
			runs.push_back(temporary_run);

			generators.emplace_back(new InstanceBasedSolutionGenerator(temporary_run));

			costs.push_back(Cost( ));

			temperature_schedulers.emplace_back(temperature_scheduler_template->clone( ));
		}

		const std::size_t T = std::min(threads_number, population_size);
		(void) T;
#pragma omp parallel for num_threads(T) if(T > 1)
		for(std::size_t i = 0; i < N; ++i)
		{
			auto& solution_generator = generators[i];
			for(const auto mutation : allowed_mutations)
			{
				solution_generator->enableMutation(mutation);
			}
			solution_generator->shuffle( );
			solution_generator->store( );
			for(std::size_t j = 0; j < N; ++j)
			{
				if(i != j)
				{
					solution_generator->addInstance(*generators[j]);
				}
			}

			const Cost initial_cost = schedule_cost_function->calculateCost(schedules[i].getSchedule( ).get( ));
			costs[i]                = initial_cost;

			auto& temperature_scheduler = temperature_schedulers[i];
			temperature_scheduler->initialize(runs[i].get( ), schedule_cost_function.get( ));
		}
	}

	void SimulatedAnnealingTSPSolver::optimizeAuto(Run& run) const
	{
		if(threads_number == 1)
		{
			optimizeIterative(run);
		}
		else
		{
			optimizeIndependent(run);
		}
	}

	void SimulatedAnnealingTSPSolver::optimizeIndependent(Run& run) const
	{
		TRACEABLE_SECTION(__optimizeIndependent__, "optimizeIndependent", logger);

		VectorSchedules schedules;
		VectorRuns runs;
		VectorCosts costs;
		VectorGenerators generators;
		VectorTemperatureSchedulers temperature_schedulers;
		initialize(run, schedules, runs, costs, generators, temperature_schedulers);

		const std::size_t N = population_size;
		const std::size_t T = std::min(threads_number, population_size);

		auto tsc_finish = [&](std::size_t group) {
			bool finish = true;
			for(std::size_t i = group; i < N; i += T)
			{
				auto& temp_sched = temperature_schedulers[i];
				if(!temp_sched->isFinish( ))
				{
					finish = false;
					break;
				}
			}
			return finish;
		};

		std::random_device random_device;
		std::mt19937_64 random_engine(random_device( ));

#pragma omp parallel for num_threads(T) if(T > 1)
		for(std::size_t j = 0; j < T; ++j)
		{
			while(!tsc_finish(j))
			{
				for(std::size_t i = j; i < N; i += T)
				{
					step(runs[i],
					     random_engine,
					     *temperature_schedulers[i],
					     *generators[i],
					     costs[i]);
				}
			}
		}

		auto best_cost_iter           = std::min_element(costs.begin( ), costs.end( ));
		ScheduleVariant& best_variant = schedules.at(std::distance(costs.begin( ), best_cost_iter));
		best_variant.apply( );
	}

	void SimulatedAnnealingTSPSolver::optimizeIterative(Run& run) const
	{
		TRACEABLE_SECTION(__optimizeIterative__, "optimizeIterative", logger);

		VectorSchedules schedules;
		VectorRuns runs;
		VectorCosts costs;
		VectorGenerators generators;
		VectorTemperatureSchedulers temperature_schedulers;
		initialize(run, schedules, runs, costs, generators, temperature_schedulers);

		const std::size_t N = population_size;
		const std::size_t T = std::min(threads_number, population_size);
		(void) T; //used for openmp

		auto tsc_finish = [&]( ) {
			bool finish = true;
			for(std::size_t i = 0; i < N; ++i)
			{
				auto& temp_sched = temperature_schedulers[i];
				if(!temp_sched->isFinish( ))
				{
					finish = false;
					break;
				}
			}
			return finish;
		};

		std::random_device random_device;
		std::mt19937_64 random_engine(random_device( ));

		while(!tsc_finish( ))
		{
#pragma omp parallel for num_threads(T) if(T > 1)
			for(std::size_t i = 0; i < N; ++i)
			{
				step(runs[i],
				     random_engine,
				     *temperature_schedulers[i],
				     *generators[i],
				     costs[i]);
			}
		}

		auto best_cost_iter           = std::min_element(costs.begin( ), costs.end( ));
		ScheduleVariant& best_variant = schedules.at(std::distance(costs.begin( ), best_cost_iter));
		best_variant.apply( );
	}

	void SimulatedAnnealingTSPSolver::step(const Run& run,
	                                       std::mt19937_64& random_engine,
	                                       TemperatureScheduler& temp_sched,
	                                       InstanceBasedSolutionGenerator& solution_generator,
	                                       Cost& best_cost) const
	{
		TRACEABLE_SECTION(__step__, "step", logger);
		if(temp_sched.isFinish( ))
		{
			return;
		}

		std::uniform_real_distribution<float> float_distribution(0.f, 1.f);
		const std::size_t S = markovChainLength(run.getWorkStops( ).size( ));
		for(size_t s = 0; s < S; ++s)
		{
			solution_generator.neighbour( );
			if(!solution_generator.hasPermutation( ))
			{
				continue;
			}

			const Cost cost = schedule_cost_function->calculateCost(run.getSchedule( ));
			if(cost < best_cost)
			{
				best_cost = cost;
				solution_generator.store( );
			}
			else
			{
				const float random_value = float_distribution(random_engine);
				const Cost delta         = cost - best_cost;
				if(acceptance(delta, random_value, temp_sched.getTemperature( )))
				{
					temp_sched.adapt(delta, random_value);
					best_cost = cost;
					solution_generator.store( );
				}
				else
				{
					solution_generator.discard( );
				}
			}
		}

		temp_sched.changeTemperature( );
	}
}
