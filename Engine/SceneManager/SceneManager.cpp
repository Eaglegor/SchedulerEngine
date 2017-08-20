#include "SceneManager.h"

#include "Scene.h"
#include "SceneContext.h"

#include <Engine/MemoryManager/ObjectSizes.h>

namespace Scheduler
{
	SceneManager::SceneManager( )
	{
	}

	SceneContext& SceneManager::createSceneContext( )
	{
		SceneContext& scene_context = *scene_contexts_factory.createObject( );
		scene_contexts.insert(scene_context);
		return scene_context;
	}

	void SceneManager::destroySceneContext(SceneContext& scene_context)
	{
		scene_contexts.erase(scene_context);
		scene_contexts_factory.destroyObject(&scene_context);
	}

	Scene& SceneManager::createScene(const SceneContext& context)
	{
		Scene& scene = *scenes_factory.createObject(context, *this);
		scenes.insert(scene);
		return scene;
	}

	void SceneManager::destroyScene(Scene& scene)
	{
		scenes.erase(scene);
		scenes_factory.destroyObject(&scene);
	}

	SceneManager::~SceneManager( )
	{
		for(Scene& scene : scenes)
		{
			scenes_factory.destroyObject(&scene);
		}
		for(SceneContext& scene_context : scene_contexts)
		{
			scene_contexts_factory.destroyObject(&scene_context);
		}
	}
}