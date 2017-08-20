#include "Evolution.h"
#include "RandomSolutionGenerator.h"
#include <Engine/SceneManager/CostFunctions/SceneCostFunction.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Utils/SceneCloner.h>

namespace Scheduler
{
	Evolution::Evolution(SceneManager& scene_manager, RandomSolutionGenerator& solution_generator, std::size_t population_size, const SceneCostFunction& cost_function)
	    : scene_manager(scene_manager),
	      solution_generator(solution_generator),
	      population_size(population_size),
	      cost_function(cost_function)
	{
		for(int i = 0; i < population_size; ++i)
		{
			population.push_back(solution_generator.generate( ));
		}
	}

	Scene& Evolution::crossover(const Scene& lhs, const Scene& rhs)
	{
		Scene& child = scene_manager.createScene(lhs.getContext( ));

		SceneCloner::cloneSceneSchedulesPool(lhs, child);
		SceneCloner::cloneSceneAssignments(lhs, child);

		return child;
	}

	void Evolution::mutate(Scene& solution)
	{
	}

	void Evolution::shrinkPopulation(std::vector<std::reference_wrapper<Scene>>& population)
	{
		std::sort(population.begin( ), population.end( ), [&](const Scene& lhs, const Scene& rhs) { return cost_function.get( ).calculateCost(lhs) < cost_function.get( ).calculateCost(rhs); });
		for(auto iter = std::next(population.begin( ), population_size); iter != population.end( ); ++iter)
		{
			scene_manager.destroyScene(*iter);
		}
		population.erase(std::next(population.begin( ), population_size), population.end( ));

		best_solution = *population.begin( );
	}

	Optional<Scene&> Evolution::getBestSolution( )
	{
		return best_solution;
	}

	void Evolution::nextIteration( )
	{
		for(int i = 0; i < population.size( ); ++i)
		{
			for(int j = 0; j < population.size( ); ++j)
			{
				if(i == j) continue;

				Scene& child = crossover(population[i], population[j]);

				mutate(child);

				population.push_back(child);
			}
		}

		shrinkPopulation(population);
	}
}