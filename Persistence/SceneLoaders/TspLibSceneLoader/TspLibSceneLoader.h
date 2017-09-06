#pragma once


#include <istream>

namespace Scheduler
{
	class TspLibRoutingService;
	class Engine;
	class Scene;

	class TspLibSceneLoader
	{
	public:
		enum class Format
		{
			XML,
			BINARY
		};

		explicit TspLibSceneLoader(Engine& engine);

		Scene& loadScene(std::istream& stream, TspLibRoutingService& routing_service, Format format, uint32_t& out_known_optimum);
		Scene& loadScene(const std::string& filename, TspLibRoutingService& routing_service, Format format, uint32_t& out_known_optimum);

	private:
		void loadBinaryMatrix(std::istream& stream, size_t& out_count, TspLibRoutingService& routing_service, uint32_t& out_known_optimum);
		void loadXmlMatrix(std::istream& stream, size_t& out_count, TspLibRoutingService& routing_service, uint32_t& out_known_optimum);

		Engine& engine;
	};
}