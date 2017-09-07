#pragma once


#include <istream>

namespace Scheduler
{
	class RoutingService;
	class Scene;
	class Engine;

	class TaillardSceneLoader
	{
	public:
		TaillardSceneLoader(Engine& engine, RoutingService& rs);

		Scene& loadScene(std::istream& stream);
		Scene& loadScene(const std::string& filename);

	private:
		Engine& engine;
		RoutingService& rs;
	};
}