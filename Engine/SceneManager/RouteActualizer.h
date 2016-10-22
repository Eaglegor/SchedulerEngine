#pragma once

namespace Scheduler
{
	class Stop;
	class RouteActualizationAlgorithm;

	class RouteActualizer
	{
	public:
		explicit RouteActualizer(Stop& stop);

		void actualize() const;

		void setDirty(bool dirty);
		
	private:
		inline const RouteActualizationAlgorithm& getAlgorithm() const;
		
		Stop& stop;
		mutable bool is_dirty;
	};
}