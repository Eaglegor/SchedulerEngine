#pragma once

#include <Engine/Utils/String.h>

namespace Scheduler
{
	class Run;
	class Schedule;
	class Scene;
	class Site;

	/** 
	 * @ingroup logging_manager
	 * 
	 * @brief Represents the single animation frame interface where the algorithms can draw their current state
	 */
	class Frame
	{
	public:
		virtual ~Frame( )
		{
		}

		/** 
		 * @brief Draws a new run on the frame
		 * 
		 * @param run The run to draw
		 */
		virtual void addRun(const Run& run)                = 0;

		/** 
		 * @brief Draws a new schedule on the frame
		 * 
		 * @param schedule The schedule to draw
		 */
		virtual void addSchedule(const Schedule& schedule) = 0;

		/** 
		 * @brief Draws a new scene on the frame
		 * 
		 * @param scene The scene to draw
		 */
		virtual void addScene(const Scene& scene)          = 0;

		/** 
		 * @brief Draws a new text on the frame
		 * 
		 * @param text The text to draw
		 */
		virtual void addText(const String& text)           = 0;

		/** 
		 * @brief Draws a new title on the frame
		 * 
		 * @param text The title to draw
		 */
		virtual void addTitle(const String& text)          = 0;

		/** 
		 * @brief Draws a new line on the frame
		 * 
		 * @param from The start point of line
		 * @param to The end point of line
		 */
		virtual void addLine(const Site& from, const Site& to) = 0;

		/** 
		 * @brief Draws a new point on the frame
		 * 
		 * @param pt The position of point
		 * @param radius The radius of point
		 */
		virtual void addPoint(const Site& pt, float radius = 3) = 0;

		/** 
		 * @brief Renders the frame into the output stream
		 */
		virtual void draw( ) = 0;
	};

	/** 
	 * @brief Represents debug renderer intefrace
	 * 
	 * @details This class is passed to the algorithms for debugging purposes. The algorithms
	 * can request the animation frames and draw their current state to simplify debugging process
	 */
	class Renderer
	{
	public:
		virtual ~Renderer( )
		{
		}

		/** 
		 * @brief Starts the next animation frame
		 * 
		 * @return The reference to the new animation frame
		 */
		virtual Frame& nextFrame( ) = 0;
	};
}