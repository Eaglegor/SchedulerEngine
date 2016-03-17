#pragma once

#include <cstddef>
#include <vector>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Stop;
	class Run;

	/** 
		This is a linear vector representation of run's stops including start and end stops which are processed differently from the work stops.
	*/
	class SCENEMANAGER_EXPORT RunStopsView
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