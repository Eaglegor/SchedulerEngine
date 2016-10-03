#pragma once

#include <Engine/SceneManager/Run.h>
#include <boost/optional.hpp>

namespace Scheduler
{
	class CWSavingsPrechecks
	{
		public:	
			static bool isFirstWorkStop(Run::WorkStopsList::iterator iter)
			{
				Run* r = (*iter)->getRun();
				return r->getWorkStops().front() == *iter;
			}
			
			static bool isLastWorkStop(Run::WorkStopsList::iterator iter)
			{
				Run* r = (*iter)->getRun();
				return r->getWorkStops().back() == *iter;
			}
			
			static bool isValidSaving(boost::optional<Run::WorkStopsList::iterator> from, boost::optional<Run::WorkStopsList::iterator> to)
			{
				return (!from || isLastWorkStop(from.value())) && (!to || isFirstWorkStop(to.value()));
			}
	};
}