#pragma once

#include <vector>
#include <Engine/Utils/ReferenceWrapper.h>
#include <Engine/Utils/Optional.h>

namespace Scheduler
{
	class SceneManager;
	class Scene;
	class RandomSolutionGenerator;
	class SceneCostFunction;
	
	class Evolution
	{
	public:
		Evolution(SceneManager& scene_manager, RandomSolutionGenerator& solution_generator, std::size_t population_size, const SceneCostFunction& cost_function);
		
		void nextIteration();
		Optional<Scene&> getBestSolution();
		
	private:
		Scene& crossover(const Scene& lhs, const Scene& rhs);
		void mutate(Scene& solution);
		void shrinkPopulation(std::vector<ReferenceWrapper<Scene>> &population);
		
		SceneManager& scene_manager;
		RandomSolutionGenerator& solution_generator;
		Optional<Scene&> best_solution;
		ReferenceWrapper<const SceneCostFunction> cost_function;
		std::vector<Scene> scene;
		std::size_t population_size;
		
		std::vector<ReferenceWrapper<Scene>> population;
	};
}