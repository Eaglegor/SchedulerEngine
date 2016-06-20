#pragma once

#include <memory>

#include <Engine/SceneManager/SceneManager.h>
#include <Engine/StrategiesManager/StrategiesManager.h>
#include <Engine/MemoryManager/MemoryManager.h>

#include <Engine_export.h>


namespace Scheduler
{
	struct EngineContext;
	
	class ENGINE_EXPORT Engine
	{
		public:
			Engine();
			Engine(const EngineContext& context);
			
			SceneManager* getSceneManager();
			StrategiesManager* getStrategiesManager();
			
		private:
			std::unique_ptr<MemoryManager> memory_manager;
			std::unique_ptr<SceneManager> scene_manager;
			std::unique_ptr<StrategiesManager> strategies_manager;
	};
}