#pragma once

namespace Scheduler
{
	class Run;
	class DurationActualizationAlgorithm;

	class DurationActualizer
	{
	public:
		explicit DurationActualizer(Run& run);

		void actualize() const;

		void setDirty(bool dirty);
		
	private:
		inline const DurationActualizationAlgorithm& getAlgorithm() const;
		
		Run& run;
		mutable bool is_dirty;
		mutable bool actualization_in_progress;
	};
}
