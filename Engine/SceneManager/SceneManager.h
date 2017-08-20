#pragma once

#include "Attribute.h"
#include "SceneObjectsFactory.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <Engine/Utils/Factory.h>

#include <Engine/Utils/ReferenceWrapper.h>
#include <functional>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Scene;
	class SceneContext;

	class SCENEMANAGER_EXPORT SceneManager
	{
	public:
		SceneManager( );
		explicit SceneManager(const SceneManager&) = delete;
		~SceneManager( );

		SceneContext& createSceneContext( );

		void destroySceneContext(SceneContext& scene_context);

		Scene& createScene(const SceneContext& context);

		void destroyScene(Scene& scene);

	private:
		SceneObjectsFactory<Scene> scenes_factory;
		SceneObjectsFactory<SceneContext> scene_contexts_factory;

		std::unordered_set<ReferenceWrapper<SceneContext>> scene_contexts;
		std::unordered_set<ReferenceWrapper<Scene>> scenes;
	};
}