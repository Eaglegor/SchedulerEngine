#include "Engine.h"

namespace Scheduler
{
	MemoryManager& Engine::getMemoryManager()
	{
		return memory_manager;
	}
	
	SceneManager& Engine::getSceneManager()
	{
		return scene_manager;
	}
	
	AlgorithmsManager& Engine::getAlgorithmsManager()
	{
		return algorithms_manager;
	}
}