#include "PngRenderer.h"

//#define cimg_use_png
#define cimg_display 0
#include "CImg.h"
#include <Engine/Concepts/Site.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <boost/filesystem.hpp>

namespace Scheduler
{
	struct Point
	{
		float x;
		float y;
		float radius;
	};
	
	struct Line
	{
		Point from;
		Point to;		
	};
	
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
		void addPoint(const Site& pt, float radius = 3) override;
		void draw() override;

	private:
		std::pair<float,float> mapXY(float x, float y)
		{
			std::pair<float,float> ret;
			ret.first=(xsize-xmargin*2)/2+xmargin+x*xscale-xshift*xscale;
			ret.second=(ysize-ymargin*2)/2+ymargin-y*yscale+yshift*yscale;
			return ret;
		}
		
		String title;
		String text;
		
		int xsize;
		int ysize;
		int xmargin;
		int ymargin;
		
		float xscale;
		float yscale;
		float xshift;
		float yshift;
		
		String filename;
		cimg_library::CImg<unsigned char> image;
		
		std::vector<Point> points;
		std::vector<Line> lines;
	};
	
	PngFrame::PngFrame(const String& filename, std::size_t width, std::size_t height):
	filename(filename),
	image(width,height,1,3,255),
	xsize(width),
	ysize(height),
	xmargin(50),
	ymargin(100)
	{
	}

	PngFrame::~PngFrame()
	{
	}

	void PngFrame::addRun(const Run& run)
	{
		addPoint(run.getStartStop().getLocation().getSite(), 5);
		addPoint(run.getEndStop().getLocation().getSite(), 5);
		for(const Stop &stop : run.getStops())
		{
			addPoint(stop.getLocation().getSite());
			addLine(stop.getLocation().getSite(), stop.getNextRoute().getTo());
		}
	}

	void PngFrame::addSchedule(const Schedule& schedule)
	{
		for(const Run &run : schedule.getRuns())
		{
			addRun(run);
		}
	}

	void PngFrame::addScene(const Scene& scene)
	{
		for(const Order& order: scene.getContext().getOrders())
		{
			if(order.getWorkOperation()) addPoint(order.getWorkOperation()->getLocation().getSite());
		}
		for(const Schedule &schedule : scene.getSchedules())
		{
			addSchedule(schedule);
		}
	}

	void PngFrame::addText(const String& text)
	{
		this->text = text;
	}

	void PngFrame::addTitle(const String& text)
	{
		this->title = text;
	}

	void PngFrame::addLine(const Site& from, const Site& to)
	{
		Line l;
		l.from.x = from.getLongitude().getValue();
		l.from.y = from.getLatitude().getValue();
		l.to.x = to.getLongitude().getValue();
		l.to.y = to.getLatitude().getValue();
		lines.push_back(l);
	}

	void PngFrame::addPoint(const Site& pt, float radius)
	{
		Point p;
		p.x = pt.getLongitude().getValue();
		p.y = pt.getLatitude().getValue();
		p.radius = radius;
		points.push_back(p);
	}

	void PngFrame::draw()
	{
		unsigned long BLACK[] = {0, 0, 0};
		unsigned long WHITE[] = {255, 255, 255};
		
		float minx = std::numeric_limits<float>::max();
		float maxx = std::numeric_limits<float>::lowest();
		float miny = std::numeric_limits<float>::max();
		float maxy = std::numeric_limits<float>::lowest();
		
		for(auto it : points)
		{
			float x = it.x;
			float y = it.y;
			minx = std::min(x, minx);
			maxx = std::max(x, maxx);
			miny = std::min(y, miny);
			maxy = std::max(y, maxy);
		}
		
		for(auto it : lines)
		{
			{
				float x = it.from.x;
				float y = it.from.y;
				minx = std::min(x, minx);
				maxx = std::max(x, maxx);
				miny = std::min(y, miny);
				maxy = std::max(y, maxy);
			}
			{
				float x = it.to.x;
				float y = it.to.y;
				minx = std::min(x, minx);
				maxx = std::max(x, maxx);
				miny = std::min(y, miny);
				maxy = std::max(y, maxy);
			}
		}
		
		xshift=(minx + maxx)/2;
		yshift=(miny + maxy)/2;
		
		float xdiff = maxx - minx;
		float ydiff = maxy - miny;
		
		float _minx = minx;
		float _miny = miny;
		float _maxx = maxx;
		float _maxy = maxy;
		
		if(minx < 0) _minx = -_minx;
		if(miny < 0) _miny = -_miny;
		
		xscale = (xsize - xmargin * 2)/std::max(xdiff, std::max(_minx, _maxx));
		yscale = (ysize - ymargin * 2)/std::max(ydiff, std::max(_miny, _maxy));
		
		for(auto it : points)
		{
			std::pair<float, float> pt = mapXY(it.x, it.y);
			image.draw_circle(static_cast<int>(pt.first), static_cast<int>(pt.second), static_cast<int>(it.radius), BLACK);
		}
		
		for(auto it : lines)
		{
			std::pair<float, float> pt1 = mapXY(it.from.x, it.from.y);
			std::pair<float, float> pt2 = mapXY(it.to.x, it.to.y);
			image.draw_line(static_cast<int>(pt1.first), static_cast<int>(pt1.second),
							static_cast<int>(pt2.first), static_cast<int>(pt2.second),
							BLACK);
		}
		
		image.draw_text(xmargin, ymargin / 2, title.c_str(), BLACK, WHITE);
		image.draw_text(xmargin, ysize - ymargin / 2, text.c_str(), BLACK, WHITE);
		
		image.save_png(filename.c_str());
	}

	PngRenderer::PngRenderer(const String& output_path, std::size_t width, std::size_t height):
	current_frame_id(0),
	output_path(output_path),
	width(width),
	height(height)
	{
		boost::filesystem::create_directories(output_path);
	}

	Frame& PngRenderer::nextFrame()
	{
		current_frame = std::make_shared<PngFrame>(output_path + "/" + generateName(current_frame_id++), width, height);
		return *current_frame;
	}

	String PngRenderer::generateName(std::size_t frame_id)
	{
		return std::to_string(frame_id) + ".png";
	}
}
