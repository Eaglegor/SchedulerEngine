#include "Engine.h"

namespace Scheduler
{
	Engine::Engine() :
		memory_manager(new MemoryManager()),
		scene_manager(new SceneManager()),
		strategies_manager(new StrategiesManager())
	{
	}

	SceneManager* Engine::getSceneManager()
	{
		return scene_manager.get();
	}
	
	StrategiesManager* Engine::getStrategiesManager()
	{
		return strategies_manager.get();
	}
}