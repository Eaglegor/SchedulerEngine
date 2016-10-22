#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Attribute.h"
#include "SceneObjectsFactory.h"

#include <Engine/Utils/Factory.h>

#include <functional>
#include <Engine/Utils/ReferenceWrapper.h>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Scene;
	class SceneContext;

    class SCENEMANAGER_EXPORT SceneManager
    {
    public:
		SceneManager();
		explicit SceneManager(const SceneManager&) = delete;
        ~SceneManager();

		SceneContext& createSceneContext();
		
		void destroySceneContext(SceneContext& scene_context);
		
        Scene& createScene(const SceneContext& context);

        void destroyScene(Scene &scene);
		
    private:
		SceneObjectsFactory<Scene> scenes_factory;
		SceneObjectsFactory<SceneContext> scene_contexts_factory;

		std::unordered_set<ReferenceWrapper<SceneContext>> scene_contexts;
        std::unordered_set<ReferenceWrapper<Scene>> scenes;
    };
}