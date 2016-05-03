#pragma once

#include <Engine/SceneManager/Extensions/ArrivalTimeActualizationAlgorithm.h>

#include <DefaultArrivalTimeActualizationAlgorithm_export.h>

namespace Scheduler
{
	/// Updates stops allocation times based on routes length, time windows and performer shift
    class DEFAULTARRIVALTIMEACTUALIZATIONALGORITHM_EXPORT DefaultArrivalTimeActualizationAlgorithm : public ArrivalTimeActualizationAlgorithm
    {
    public:
	    static constexpr const char* staticGetName() { return "Default"; }
		virtual const char* getName() const { return staticGetName(); };

		virtual void actualize(Schedule* schedule) override;
    };
}