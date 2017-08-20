#pragma once

#include <random>

namespace Scheduler
{
	class Scene;
	class SceneContext;
	class SceneManager;

	class RandomSolutionGenerator
	{
	public:
		RandomSolutionGenerator(const Scene& base_scene);
		Scene& generate( );

	private:
		std::mt19937 g;
		const Scene& base_scene;
	};
}