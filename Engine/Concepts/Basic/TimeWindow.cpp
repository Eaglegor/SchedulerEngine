#include "TimeWindow.h"

namespace Scheduler
{

	TimeWindow::TimeWindow() : start_time(TimePoint::min()),
							   end_time(TimePoint::max()) {
	}

	TimeWindow::TimeWindow(const TimeWindow &rhs) : start_time(rhs.start_time),
													end_time(rhs.end_time) {
	}

	TimeWindow::TimeWindow(const TimePoint& start_time, const TimePoint& end_time) : start_time(start_time),
																					 end_time(end_time) {
	}

	const TimePoint& TimeWindow::getStartTime() const {
		return start_time;
	}

	const TimePoint& TimeWindow::getEndTime() const {
		return end_time;
	}

	void TimeWindow::setStartTime(const TimePoint& start_time) {
		this->start_time = start_time;
	}

	void TimeWindow::setEndTime(const TimePoint& end_time) {
		this->end_time = end_time;
	}

	Duration TimeWindow::getDuration() const {
		return end_time - start_time;
	}

	bool TimeWindow::operator==(const TimeWindow& rhs) const {
		return start_time == rhs.start_time && end_time == rhs.end_time;
	}

	bool TimeWindow::operator!=(const TimeWindow& rhs) const {
		return !(*this == rhs);
	}

	TimeWindow& TimeWindow::operator=(const TimeWindow &rhs) {
		this->start_time = rhs.start_time;
		this->end_time = rhs.end_time;
		return *this;
	}
}