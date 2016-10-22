#pragma once

namespace Scheduler
{
	class Schedule;
	class ArrivalTimeActualizationAlgorithm;

	class ArrivalTimeActualizer
	{
	public:
		explicit ArrivalTimeActualizer(Schedule& schedule);

		void actualize() const;

		void setDirty(bool dirty);
		
	private:
		inline const ArrivalTimeActualizationAlgorithm& getAlgorithm() const;
		
		mutable bool is_dirty;
		mutable bool actualization_in_progress;
		Schedule& schedule;
	};
}
