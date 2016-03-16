#include "Engine.h"

#include "EngineContext.h"

namespace Scheduler
{
	Engine::Engine(const EngineContext& context):
	memory_manager(new MemoryManager()),
	scene_manager(new SceneManager(context.routing_service, memory_manager.get())),
	strategies_manager(new StrategiesManager(memory_manager.get()))
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