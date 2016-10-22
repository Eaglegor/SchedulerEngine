#include "Engine.h"

namespace Scheduler
{
	SceneManager& Engine::getSceneManager()
	{
		return scene_manager;
	}
	
	AlgorithmsManager& Engine::getAlgorithmsManager()
	{
		return algorithms_manager;
	}
}