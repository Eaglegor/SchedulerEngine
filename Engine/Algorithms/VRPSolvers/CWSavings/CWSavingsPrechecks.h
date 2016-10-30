#pragma once

#include <Engine/SceneManager/Run.h>
#include <boost/optional.hpp>

namespace Scheduler
{
	class CWSavingsPrechecks
	{
		public:	
			static bool isFirstWorkStop(Run::ConstWorkStopIterator iter)
			{
				const Run& r = iter->getRun();
				return r.getWorkStops().begin() == iter;
			}
			
			static bool isLastWorkStop(Run::ConstWorkStopIterator iter)
			{
				const Run& r = iter->getRun();
				return std::prev(r.getWorkStops().end()) == iter;
			}
			
			static bool isValidSaving(boost::optional<Run::ConstWorkStopIterator> from, boost::optional<Run::ConstWorkStopIterator> to)
			{
				if(from && to && from.value()->getRun() == to.value()->getRun()) return false;
				return (!from || isLastWorkStop(from.value())) && (!to || isFirstWorkStop(to.value()));
			}
	};
}