#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <Engine/Concepts/Basic/SpeedDurationDistanceOperations.h>

TEST_CASE("Concepts - Basic - SpeedDurationDistanceOperations", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Distance = speed * duration")
	{
		Speed speed(Distance(20.0f));
		Duration duration(30);

		Distance distance = speed * duration;

		REQUIRE(distance == Distance(600));
	}

	SECTION("Distance = duration * speed")
	{
		Speed speed(Distance(20.0f));
		Duration duration(30);

		Distance distance = duration * speed;

		REQUIRE(distance == Distance(600));
	}

	SECTION("Duration = distance / speed")
	{
		Distance distance(100.0f);
		Speed speed(Distance(50.0f));

		Duration duration = distance / speed;

		REQUIRE(duration == Duration(2));

		speed.setValue(Distance(60.0f));
		duration = distance / speed;

		REQUIRE(duration == Duration(2));

		speed.setValue(Distance(80.0f));
		duration = distance / speed;

		REQUIRE(duration == Duration(1));
	}

	SECTION("Speed = distance / duration")
	{
		Distance distance(100.0f);
		Duration duration(2);

		Speed speed = distance / duration;

		REQUIRE(speed == Speed(Distance(50.0f)));
	}
}