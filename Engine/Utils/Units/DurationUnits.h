#pragma once

#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	namespace Units
	{
		static Duration seconds(long count)
		{
			return Duration(count);
		}

		static Duration minutes(long count)
		{
			return Duration(count * 60);
		}

		static Duration hours(long count)
		{
			return Duration(count * 3600);
		}

		static Duration days(long count)
		{
			return Duration(count * 86400);
		}

		static Duration minutes_seconds(long minutes_count, long seconds_count)
		{
			return minutes(minutes_count) + seconds(seconds_count);
		}

		static Duration hours_minutes(long hours_count, long minutes_count)
		{
			return hours(hours_count) + minutes(minutes_count);
		}

		static Duration hours_minutes_seconds(long hours_count, long minutes_count, long seconds_count)
		{
			return hours(hours_count) + minutes(minutes_count) + seconds(seconds_count);
		}
	}
}