#pragma once

#include <memory>

#include <Engine/SceneManager/SceneManager.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Engine/MemoryManager/MemoryManager.h>

#include <Engine_export.h>


namespace Scheduler
{
	struct EngineContext;
	
	class ENGINE_EXPORT Engine
	{
		public:
			SceneManager& getSceneManager();
			AlgorithmsManager& getAlgorithmsManager();
			
		private:
			SceneManager scene_manager;
			AlgorithmsManager algorithms_manager;
	};
}