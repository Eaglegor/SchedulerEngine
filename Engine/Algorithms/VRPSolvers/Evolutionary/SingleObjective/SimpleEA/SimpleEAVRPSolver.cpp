#include "SimpleEAVRPSolver.h"
#include "Evolution.h"
#include "RandomSolutionGenerator.h"
#include <Engine/SceneManager/CostFunctions/SceneCostFunction.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Utils/SceneCloner.h>

namespace Scheduler
{
	void SimpleEAVRPSolver::optimize(Scene& scene) const
	{
		RandomSolutionGenerator random_solution_generator(scene);

		Evolution evolution(scene.getSceneManager( ), random_solution_generator, population_size, cost_function.get( ));

		bool finished                = false;
		std::size_t iterations_count = 0;

		while(!finished)
		{
			evolution.nextIteration( );

			if(++iterations_count >= iterations_limit) finished = true;
		}

		Optional<Scene&> best_solution = evolution.getBestSolution( );
		if(best_solution) SceneCloner::cloneSceneAssignments(best_solution.get( ), scene);
	}

	void SimpleEAVRPSolver::setCostFunction(const SceneCostFunction& cost_function)
	{
		this->cost_function = cost_function;
	}

	void SimpleEAVRPSolver::setIterationsLimit(std::size_t iterations_limit)
	{
		this->iterations_limit = iterations_limit;
	}

	void SimpleEAVRPSolver::setPopulationSize(std::size_t population_size)
	{
		this->population_size = population_size;
	}
}