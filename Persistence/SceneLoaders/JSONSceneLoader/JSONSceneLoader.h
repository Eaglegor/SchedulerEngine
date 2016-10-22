#pragma once

#include <JSONSceneLoader_export.h>
#include <istream>

namespace Scheduler
{
	class RoutingService;
	class Scene;
	class Engine;

	class JSONSCENELOADER_EXPORT JSONSceneLoader
	{
	public:
		JSONSceneLoader(Engine& engine, RoutingService& rs);

		Scene& loadScene(std::istream& stream);
		Scene& loadScene(const std::string &filename);

	private:
		Engine& engine;
		RoutingService& routing_service;
	};
}