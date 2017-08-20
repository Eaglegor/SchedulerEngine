#pragma once

#include <memory>

#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Engine/MemoryManager/MemoryManager.h>
#include <Engine/SceneManager/SceneManager.h>

#include <Core_export.h>


namespace Scheduler
{
	struct EngineContext;

	class CORE_EXPORT Engine
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