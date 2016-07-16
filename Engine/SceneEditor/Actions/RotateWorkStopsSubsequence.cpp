#include "RotateWorkStopsSubsequence.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include "ActionsImpl.h"
#include <assert.h>

namespace Scheduler
{
    RotateWorkStopsSubsequence::RotateWorkStopsSubsequence(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator middle_stop, WorkStopIterator end_stop) :
		schedule((*run_iterator)->getSchedule()),
		run_index(std::distance<ImmutableVector<Run*>::const_iterator>(schedule->getRuns().begin(), run_iterator)),
		start_index(std::distance<ImmutableVector<WorkStop*>::const_iterator>((*run_iterator)->getWorkStops().begin(), start_stop)),
        middle_index(std::distance<ImmutableVector<WorkStop*>::const_iterator>((*run_iterator)->getWorkStops().begin(), middle_stop)),
		end_index(std::distance<ImmutableVector<WorkStop*>::const_iterator>((*run_iterator)->getWorkStops().begin(), end_stop))
	{
		assert(run_index >= 0);
		assert(run_index < schedule->getRuns().size());
		assert(start_index >= 0);
		assert(start_index < (*run_iterator)->getWorkStops().size());
		assert(end_index >= start_index);
        assert(middle_index >= start_index);
        assert(end_index >= middle_index);
		assert(end_index <= (*run_iterator)->getWorkStops().size());
	}

	void RotateWorkStopsSubsequence::perform()
	{
		if (start_index == end_index || start_index == middle_index || end_index == middle_index) return;

		Run* r = schedule->getRuns()[run_index];

        ptrdiff_t first = start_index;
        ptrdiff_t n_first = middle_index;
        ptrdiff_t last = end_index;
        ptrdiff_t next = n_first;
        while (first != next) {
            ActionsImpl::swapRunWorkStops(r, first++, next++);
            if (next == last) {
                next = n_first;
            } else if (first == n_first) {
                n_first = next;
            }
        }
	}

	void RotateWorkStopsSubsequence::rollback()
	{
        if (start_index == end_index || start_index == middle_index || end_index == middle_index) return;

        Run* r = schedule->getRuns()[run_index];

        ptrdiff_t first = end_index - 1;
        ptrdiff_t n_first = end_index - 1 - middle_index + start_index;
        ptrdiff_t last = start_index - 1;
        ptrdiff_t next = n_first;

        while (first != next) {
            ActionsImpl::swapRunWorkStops(r, first--, next--);
            if (next == last) {
                next = n_first;
            } else if (first == n_first) {
                n_first = next;
            }
        }
	}
}
