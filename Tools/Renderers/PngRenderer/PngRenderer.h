#pragma once

#include <Engine/LoggingManager/Renderer.h>
#include <Engine/Utils/String.h>
#include <Engine/Utils/Optional.h>

#include <PngRenderer_export.h>

#define cimg_use_png
#include "CImg.h"

namespace Scheduler
{
	class PNGRENDERER_EXPORT PngRenderer : public Renderer
	{
	private:
		class PngFrame : public Frame
		{
		public:
			PngFrame(const String& filename, std::size_t width, std::size_t height);
			~PngFrame();

			void addRun(const Run& run) override;
			void addSchedule(const Schedule& schedule) override;
			void addScene(const Scene& scene) override;
			void addText(const String& text) override;
			void addTitle(const String& text) override;
			void addLine(const Site& from, const Site& to) override;
			void addPoint(const Site& pt) override;
			void draw() override;

		private:
			String filename;
			cimg_library::CImg<unsigned char> image;
		};

	public:
		explicit PngRenderer(const String& output_path);

		void setResolution(std::size_t width, std::size_t height);

		Frame& nextFrame() override;

	private:
		static String generateName(std::size_t frame_id);

		std::size_t current_frame_id;
		String output_path;
		Optional<PngFrame> current_frame;

		std::size_t width;
		std::size_t height;
	};
}
