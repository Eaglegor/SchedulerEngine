#pragma once

#include <cstdint>
#include <vector>

namespace Scheduler
{
	class Stop;
	class Run;

	class RunStopsView
	{
	public:
		RunStopsView(Run* run);

		std::vector<Stop*>::iterator begin();
		std::vector<Stop*>::iterator end();

		std::vector<Stop*>::const_iterator begin() const;
		std::vector<Stop*>::const_iterator end() const;

		size_t size() const;
		bool empty() const;

	private:
		std::vector<Stop*> stops;
	};
}