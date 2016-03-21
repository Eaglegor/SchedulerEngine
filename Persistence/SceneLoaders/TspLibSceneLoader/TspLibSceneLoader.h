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
		enum class Format
		{
			XML,
			BINARY
		};

		TspLibSceneLoader(SceneManager* scene_manager);

		Scene* loadScene(std::istream& stream, TspLibRoutingService* routing_service, Format format, uint32_t &out_known_optimum);
		Scene* loadScene(const std::string &filename, TspLibRoutingService* routing_service, Format format, uint32_t &out_known_optimum);

	private:
		void loadBinaryMatrix(std::istream& stream, size_t &out_count, TspLibRoutingService* routing_service, uint32_t &out_known_optimum);
		void loadXmlMatrix(std::istream& stream, size_t &out_count, TspLibRoutingService* routing_service, uint32_t &out_known_optimum);

		SceneManager* scene_manager;
	};
}