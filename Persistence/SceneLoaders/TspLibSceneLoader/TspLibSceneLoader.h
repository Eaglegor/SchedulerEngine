#pragma once

#include <TspLibSceneLoader_export.h>
#include <istream>

namespace Scheduler
{
	class TspLibRoutingService;
	class SceneManager;
	class Scene;

	class TSPLIBSCENELOADER_EXPORT TspLibSceneLoader
	{
	public:
		TspLibSceneLoader(SceneManager* scene_manager);

		Scene* loadScene(std::istream& stream, TspLibRoutingService* routing_service);
		Scene* loadScene(const std::string &filename, TspLibRoutingService* routing_service);

	private:
		SceneManager* scene_manager;
	};
}