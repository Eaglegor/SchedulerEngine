#pragma once

#include <JSONSceneLoader_export.h>
#include <istream>

namespace Scheduler
{
	class SceneManager;
	class Scene;

	class JSONSCENELOADER_EXPORT JSONSceneLoader
	{
	public:
		JSONSceneLoader(SceneManager* scene_manager);

		Scene* loadScene(std::istream& stream);
		Scene* loadScene(const std::string &filename);

	private:
		SceneManager* scene_manager;
	};
}