#include "ScrambleWorkStops.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/WorkStop.h>
#include <assert.h>
#include <random>

namespace Scheduler
{
	ScrambleWorkStops::ScrambleWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last)
	    : run(run),
	      first(first),
	      last(last),
	      performed(false)
	{
	}

	void ScrambleWorkStops::perform( )
	{
		assert(!performed);
		if(first == std::prev(last) || first == last) return;

		if(swaps.empty( ))
		{
			std::random_device random_device;
			std::mt19937_64 random_engine(random_device( ));
			std::uniform_int_distribution<size_t> shuffle_distribution;
			const std::size_t n = std::distance(first, last);
			auto current_iter   = first;
			for(std::size_t i = n - 1; i > 0; --i)
			{
				const std::size_t k = shuffle_distribution(random_engine, std::uniform_int_distribution<std::size_t>::param_type(0, i));
				auto lhs            = std::next(current_iter, i);
				auto rhs            = std::next(current_iter, k);
				swaps.emplace_back(run, std::next(current_iter, i), std::next(current_iter, k));
				swaps.back( ).perform( );
				if(i == 0) current_iter = rhs;
				if(k == 0) current_iter = lhs;
			}
		}
		else
		{
			for(SwapWorkStops& action : swaps)
			{
				action.perform( );
			}
		}

		performed = true;
	}

	void ScrambleWorkStops::rollback( )
	{
		assert(performed);

		for(auto iter = swaps.rbegin( ); iter != swaps.rend( ); ++iter)
		{
			iter->rollback( );
		}

		performed = false;
	}
}
