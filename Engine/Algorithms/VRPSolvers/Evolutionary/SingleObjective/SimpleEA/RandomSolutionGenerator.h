#pragma once

namespace Scheduler
{
	class Scene;
	class SceneContext;
	class SceneManager;
	
	class RandomSolutionGenerator
	{
	public:
		static Scene& generate(Scene& base_scene);
	};
}