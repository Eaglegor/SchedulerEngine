#include "PngRenderer.h"

namespace Scheduler
{
	PngRenderer::PngFrame::PngFrame(const String& filename, std::size_t width, std::size_t height):
	filename(filename),
	image(width, height,1,3)
	{
	}

	PngRenderer::PngFrame::~PngFrame()
	{
	}

	void PngRenderer::PngFrame::addRun(const Run& run)
	{
	}

	void PngRenderer::PngFrame::addSchedule(const Schedule& schedule)
	{
	}

	void PngRenderer::PngFrame::addScene(const Scene& scene)
	{
	}

	void PngRenderer::PngFrame::addText(const String& text)
	{
	}

	void PngRenderer::PngFrame::addTitle(const String& text)
	{
	}

	void PngRenderer::PngFrame::addLine(const Site& from, const Site& to)
	{
	}

	void PngRenderer::PngFrame::addPoint(const Site& pt)
	{
	}

	void PngRenderer::PngFrame::draw()
	{
	}

	PngRenderer::PngRenderer(const String& output_path):
	current_frame_id(0),
	output_path(output_path),
	width(800),
	height(600)
	{
	}

	void PngRenderer::setResolution(std::size_t width, std::size_t height)
	{
		this->width = width;
		this->height = height;
	}

	Frame& PngRenderer::nextFrame()
	{
		current_frame.emplace(generateName(current_frame_id), width, height);
		return current_frame.get();
	}

	String PngRenderer::generateName(std::size_t frame_id)
	{
		return std::to_string(frame_id) + ".png";
	}
}
