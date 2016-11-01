#pragma once

#include "../Run.h"
#include <functional>
#include <vector>

#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT InvariantWorkStopsList
	{
	public:
		using const_iterator = std::vector<ReferenceWrapper<const WorkStop>>::const_iterator;
		
		InvariantWorkStopsList() = default;
		InvariantWorkStopsList(Run::WorkStopsList &stops);

		InvariantWorkStopsList& operator=(InvariantWorkStopsList &&rhs);
		InvariantWorkStopsList& operator=(const Run::WorkStopsList &rhs);
		
		const_iterator begin() const;
		const_iterator end() const;
		
		Run::ConstWorkStopIterator getRunWorkStopIterator(const Run& r, const_iterator iter);
		
	private:
		std::vector<ReferenceWrapper<const WorkStop>> work_stops;
	};
}