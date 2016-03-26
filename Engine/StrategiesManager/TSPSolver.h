#pragma once

namespace Scheduler
{
	class Run;
	class Schedule;

	/// Travelling salesman problem solver
	class TSPSolver
	{
	public:
		virtual ~TSPSolver(){}

		/// This method is allowed to optimize the whole schedule involving runs creation/destruction, moving stops between runs etc.
		virtual void optimize(Schedule* schedule) const = 0;

		/// This method is allowed to optimize only a single run defined by index
		virtual void optimize(Run* run) const = 0;

		virtual const char* getName() const = 0;
	};
}