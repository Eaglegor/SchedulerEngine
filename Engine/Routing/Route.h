#pragma once

#include "Distance.h"
#include "Duration.h"
#include "Site.h"

#include <Concepts_export.h>

namespace Scheduler
{
	class CONCEPTS_EXPORT Route
	{
	public:
		Route( );
		Route(const Route& rhs);
		Route(const Site& from, const Site& to, const Distance& distance, const Duration& duration);

		const Site& getFrom( ) const;
		const Site& getTo( ) const;

		void setFrom(const Site& from);
		void setTo(const Site& to);

		const Distance& getDistance( ) const;
		void setDistance(const Distance& distance);

		const Duration& getDuration( ) const;
		void setDuration(const Duration& duration);

		bool operator==(const Route& rhs) const;
		bool operator!=(const Route& rhs) const;

		Route& operator=(const Route& rhs);

	private:
		Site from;
		Site to;

		Distance distance;
		Duration duration;
	};
}