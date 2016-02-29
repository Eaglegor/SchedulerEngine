#pragma once

#include <Engine/Concepts/Basic/Duration.h>

namespace Scheduler
{
	namespace Units
	{
		Duration seconds(long count)
		{
			return Duration(count);
		}

		Duration minutes(long count)
		{
			return Duration(count * 60);
		}

		Duration hours(long count)
		{
			return Duration(count * 3600);
		}

		Duration days(long count)
		{
			return Duration(count * 86400);
		}

		Duration minutes_seconds(long minutes_count, long seconds_count)
		{
			return minutes(minutes_count) + seconds(seconds_count);
		}

		Duration hours_minutes(long hours_count, long minutes_count)
		{
			return hours(hours_count) + minutes(minutes_count);
		}

		Duration hours_minutes_seconds(long hours_count, long minutes_count, long seconds_count)
		{
			return hours(hours_count) + minutes(minutes_count) + seconds(seconds_count);
		}
	}

}