#pragma once

#include <TaillardSceneLoader_export.h>
#include <istream>

namespace Scheduler
{
	class RoutingService;
	class Scene;
	class Engine;

	class TAILLARDSCENELOADER_EXPORT TaillardSceneLoader
	{
	public:
		TaillardSceneLoader(Engine& engine, RoutingService& rs);

		Scene& loadScene(std::istream& stream);
		Scene& loadScene(const std::string &filename);

	private:
		Engine& engine;
		RoutingService& rs;
	};
}