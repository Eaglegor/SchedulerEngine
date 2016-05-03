#include "ScrambleWorkStopsSubsequence.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include "ActionsImpl.h"
#include <assert.h>

namespace Scheduler
{
    ScrambleWorkStopsSubsequence::ScrambleWorkStopsSubsequence(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator end_stop):
		schedule((*run_iterator)->getSchedule()),
		ir(std::distance<ImmutableVector<Run*>::const_iterator>(schedule->getRuns().begin(), run_iterator)),
		ia(std::distance<ImmutableVector<WorkStop*>::const_iterator>((*run_iterator)->getWorkStops().begin(), start_stop)),
		ib(std::distance<ImmutableVector<WorkStop*>::const_iterator>((*run_iterator)->getWorkStops().begin(), end_stop))
    {
	}

    void ScrambleWorkStopsSubsequence::perform()
	{
		if (ia == ib) return;

		Run* r = schedule->getRuns()[ir];
        std::random_device random_device;
        std::mt19937_64 random_engine(random_device());
        std::uniform_int_distribution<size_t> shuffle_distribution;
        rollback_indexes.clear();
        const size_t n = ib - ia;
        for (size_t i = n - 1; i > 0; --i) {
            const size_t k = shuffle_distribution(random_engine, std::uniform_int_distribution<size_t>::param_type(0, i));
            ActionsImpl::swapRunWorkStops(r, ia + i, ia + k);
            rollback_indexes.push_back(k);
        }
	}

    void ScrambleWorkStopsSubsequence::rollback()
	{
        Run* r = schedule->getRuns()[ir];
        size_t i = 1;
        for (auto it = rollback_indexes.rbegin(); it != rollback_indexes.rend(); ++it) {
            ActionsImpl::swapRunWorkStops(r, ia + i, ia + *it);
            ++i;
        }
        rollback_indexes.clear();
	}
}
