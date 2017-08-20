#pragma once

#include "Speed.h"

#include <Concepts_export.h>

namespace Scheduler
{
	/// Vehicle routing settings (vehicle type, average speed etc.) - used by routing service to build correct routes
	class CONCEPTS_EXPORT RoutingProfile
	{
	public:
		RoutingProfile( );
		RoutingProfile(const RoutingProfile& rhs);

		const Speed& getAverageSpeed( ) const;
		void setAverageSpeed(const Speed& average_speed);

		bool operator==(const RoutingProfile& rhs) const;
		bool operator!=(const RoutingProfile& rhs) const;

		RoutingProfile& operator=(const RoutingProfile& rhs);

	private:
		Speed average_speed;
	};
}