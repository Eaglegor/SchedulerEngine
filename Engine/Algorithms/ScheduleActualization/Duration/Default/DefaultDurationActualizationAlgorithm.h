#pragma once

#include <Engine/SceneManager/Algorithms/Actualization/DurationActualizationAlgorithm.h>

#include <DefaultDurationActualizationAlgorithm_export.h>

namespace Scheduler
{
	/// Updates stops durations based on the operations scheduled to be performed at the stop
	class DEFAULTDURATIONACTUALIZATIONALGORITHM_EXPORT DefaultDurationActualizationAlgorithm : public DurationActualizationAlgorithm
	{
	public:
		static constexpr const char* staticGetName( )
		{
			return "Default";
		}
		virtual const char* getName( ) const
		{
			return staticGetName( );
		};

		virtual void actualize(Run& run) const override;
	};
}