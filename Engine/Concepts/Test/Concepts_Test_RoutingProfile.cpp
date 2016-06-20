#include <catch.hpp>

#include <Engine/Concepts/RoutingProfile.h>
#include <Engine/Utils/Units/SpeedUnits.h>

TEST_CASE("Concepts - Basic - RoutingProfile", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Default constructor")
	{
		RoutingProfile r;

		REQUIRE(r.getAverageSpeed() == Units::speedPerHour(60.0f));
	}

	SECTION("Copy constructor")
	{
		RoutingProfile r;

		r.setAverageSpeed(Units::speedPerHour(30.0f));

		RoutingProfile r2(r);
		RoutingProfile r3 = r2;

		REQUIRE(r.getAverageSpeed() == r2.getAverageSpeed());
		REQUIRE(r2.getAverageSpeed() == r3.getAverageSpeed());
	}

	SECTION("Getters and setters")
	{
		RoutingProfile r;

		r.setAverageSpeed(Units::speedPerHour(45.0f));

		REQUIRE(r.getAverageSpeed() == Units::speedPerHour(45.0f));
	}

	SECTION("Comparison operators")
	{
		RoutingProfile r1, r2, r3;

		r1.setAverageSpeed(Units::speedPerHour(30.0f));
		r2.setAverageSpeed(Units::speedPerHour(15.0f));
		r3.setAverageSpeed(Units::speedPerHour(30.0f));

		REQUIRE(r1 == r3);
		REQUIRE(r1 != r2);
	}

	SECTION("Assignment operator")
	{
		RoutingProfile r1, r2;

		r1.setAverageSpeed(Units::speedPerHour(30.0f));

		r2 = r1;

		REQUIRE(r1 == r2);
	}

}