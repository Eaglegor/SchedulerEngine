#include <catch.hpp>

#include <Engine/Utils/Units/DurationUnits.h>

TEST_CASE("Concepts - Basic - DurationUnits", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Seconds")
	{
		REQUIRE(Units::seconds(1) == Duration(1));
	}

	SECTION("Minutes")
	{
		REQUIRE(Units::minutes(1) == Duration(60));
	}

	SECTION("Hours")
	{
		REQUIRE(Units::hours(1) == Duration(3600));
	}

	SECTION("Days")
	{
		REQUIRE(Units::days(1) == Duration(86400));
	}

	SECTION("Minutes + seconds")
	{
		REQUIRE(Units::minutes_seconds(1, 30) == Duration(90));
	}

	SECTION("Hours + minutes")
	{
		REQUIRE(Units::hours_minutes(1, 30) == Duration(5400));
	}

	SECTION("Hours + minutes + seconds")
	{
		REQUIRE(Units::hours_minutes_seconds(1, 30, 30) == Duration(5430));
	}
}