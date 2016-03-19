#pragma once

#include <Engine/SceneManager/Extensions/TimeWindowSelector.h>

#include <FirstTimeWindowSelector_export.h>

namespace Scheduler
{
	/// Selects the first of the multiple stop time windows or creates a broad time window if there are no time windows specified for any stop's operation
	class FIRSTTIMEWINDOWSELECTOR_EXPORT FirstTimeWindowSelector : public TimeWindowSelector
	{
	public:
		virtual TimeWindow selectTimeWindow(Stop *stop) override;

		static constexpr const char* staticGetName() { return "First"; }
		virtual const char* getName() const override { return staticGetName(); };
	};
}