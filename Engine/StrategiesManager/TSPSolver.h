#pragma once

#include <cstddef>

namespace Scheduler
{
	class Schedule;

	/// Travelling salesman problem solver
	class TSPSolver
	{
	public:
		virtual ~TSPSolver(){}

		/// This method is allowed to optimize the whole schedule involving runs creation/destruction, moving stops between runs etc.
		virtual void optimize(Schedule* schedule) const = 0;

		/// This method is allowed to optimize only a single run defined by index
		virtual void optimize(Schedule* schedule, size_t run_index) const = 0;

		virtual const char* getName() const = 0;
	};
}