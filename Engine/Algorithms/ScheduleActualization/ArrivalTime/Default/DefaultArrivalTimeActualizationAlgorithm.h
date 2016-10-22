#pragma once

#include <Engine/SceneManager/Algorithms/Actualization/ArrivalTimeActualizationAlgorithm.h>

#include <DefaultArrivalTimeActualizationAlgorithm_export.h>

namespace Scheduler
{
    class DEFAULTARRIVALTIMEACTUALIZATIONALGORITHM_EXPORT DefaultArrivalTimeActualizationAlgorithm : public ArrivalTimeActualizationAlgorithm
    {
    public:
	    static constexpr const char* staticGetName() { return "Default"; }
		virtual const char* getName() const { return staticGetName(); };

		virtual void actualize(Schedule& schedule) const override;
    };
}