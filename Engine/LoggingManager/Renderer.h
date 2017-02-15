#pragma once

#include <Engine/Utils/String.h>

namespace Scheduler
{
	class Run;
	class Schedule;
	class Scene;
	class Site;

	class Frame
	{
	public:
		virtual ~Frame() {}

		virtual void addRun(const Run& run) = 0;
		virtual void addSchedule(const Schedule& schedule) = 0;
		virtual void addScene(const Scene& scene) = 0;
		virtual void addText(const String& text) = 0;
		virtual void addTitle(const String& text) = 0;
		virtual void addLine(const Site& from, const Site& to) = 0;
		virtual void addPoint(const Site& pt, float radius = 3) = 0;

		virtual void draw() = 0;
	};

	class Renderer
	{
	public:
		virtual ~Renderer() {}
		
		virtual Frame& nextFrame() = 0;
	};
}