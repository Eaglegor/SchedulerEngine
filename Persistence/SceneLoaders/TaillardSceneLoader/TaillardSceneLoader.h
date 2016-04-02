#pragma once

#include <TaillardSceneLoader_export.h>
#include <istream>

namespace Scheduler
{
	class RoutingService;
	class SceneManager;
	class Scene;

	class TAILLARDSCENELOADER_EXPORT TaillardSceneLoader
	{
	public:
		TaillardSceneLoader(SceneManager* scene_manager, RoutingService* rs);

		Scene* loadScene(std::istream& stream);
		Scene* loadScene(const std::string &filename);

	private:
		RoutingService* rs;
		SceneManager* scene_manager;
	};
}