#include "RunStopsView.h"
#include <Engine/SceneManager/Run.h>


namespace Scheduler
{

	RunStopsView::RunStopsView(Run *run) {
		stops.push_back(run->getStartStop());
		for(Stop* stop: run->getWorkStops())
		{
			stops.push_back(stop);
		}
		stops.push_back(run->getEndStop());
	}

	std::vector<Stop *>::iterator RunStopsView::begin() {
		return stops.begin();
	}

	std::vector<Stop *>::iterator RunStopsView::end() {
		return stops.end();
	}

	std::vector<Stop *>::const_iterator RunStopsView::begin() const {
		return stops.begin();
	}

	std::vector<Stop *>::const_iterator RunStopsView::end() const {
		return stops.end();
	}

	size_t RunStopsView::size() const {
		return stops.size();
	}

	bool RunStopsView::empty() const {
		return stops.empty();
	}
}