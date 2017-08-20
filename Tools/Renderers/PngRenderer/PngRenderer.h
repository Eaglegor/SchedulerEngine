#pragma once

#include <Engine/LoggingManager/Renderer.h>
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/String.h>
#include <memory>

#include <PngRenderer_export.h>

namespace Scheduler
{
	class PngFrame;

	class PNGRENDERER_EXPORT PngRenderer : public Renderer
	{
	public:
		explicit PngRenderer(const String& output_path, std::size_t width = 800, std::size_t height = 600);

		Frame& nextFrame( ) override;

	private:
		static String generateName(std::size_t frame_id);

		std::size_t current_frame_id;
		String output_path;
		std::shared_ptr<Frame> current_frame;

		std::size_t width;
		std::size_t height;
	};
}
