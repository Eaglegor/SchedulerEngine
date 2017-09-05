#pragma once

#include <memory>

#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Engine/MemoryManager/MemoryManager.h>
#include <Engine/SceneManager/SceneManager.h>




namespace Scheduler
{
	struct EngineContext;

	class Engine
	{
	public:
		MemoryManager& getMemoryManager( );
		SceneManager& getSceneManager( );
		AlgorithmsManager& getAlgorithmsManager( );

	private:
		MemoryManager memory_manager;
		SceneManager scene_manager;
		AlgorithmsManager algorithms_manager;
	};
}