#pragma once

#include "Attribute.h"
#include "SceneObjectsFactory.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <Engine/Utils/Factory.h>

#include <Engine/Utils/ReferenceWrapper.h>
#include <functional>



namespace Scheduler
{
	class Scene;
	class SceneContext;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Scene manager is a class that creates and owns scenes and scene contexts
	 * 
	 */
	class SceneManager
	{
	public:
		/**
		 * @brief Constructor
		 */
		SceneManager( );

		explicit SceneManager(const SceneManager&) = delete;
		
		/**
		 * @brief Destructor
		 */
		~SceneManager( );

		/**
		 * @brief Creates new scene context
		 * 
		 * @return Reference to the created scene context
		 */
		SceneContext& createSceneContext( );

		/**
		 * @brief Destroys the scene context
		 * 
		 * @warning Scene context must be created by this scene manager.
		 * Passing scene context that wasn't created by this scene manager causes undefined behavior
		 * 
		 * @param scene_context Scene context to destroy
		 */
		void destroySceneContext(SceneContext& scene_context);

		/**
		 * @brief Creates new scene
		 * 
		 * @param context Scene context to create the scene for
		 * 
		 * @return Reference to the created scene
		 */
		Scene& createScene(const SceneContext& context);

		/**
		 * @brief Destroys the scene
		 * 
		 * @warning Scene must be created by this scene manager.
		 * Passing scene that wasn't created by this scene manager causes undefined behavior
		 * 
		 * @param scene Scene to destroy
		 */
		void destroyScene(Scene& scene);

	private:
		SceneObjectsFactory<Scene> scenes_factory;
		SceneObjectsFactory<SceneContext> scene_contexts_factory;

		std::unordered_set<ReferenceWrapper<SceneContext>> scene_contexts;
		std::unordered_set<ReferenceWrapper<Scene>> scenes;
	};
}