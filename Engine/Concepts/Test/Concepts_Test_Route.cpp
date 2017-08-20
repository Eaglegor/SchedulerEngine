#include <catch.hpp>

#include "MakeLocation.h"
#include <Engine/Concepts/Route.h>

TEST_CASE("Concepts - Basic - Route", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Default constructor")
	{
		Route r;

		REQUIRE(r.getFrom( ) == Site( ));
		REQUIRE(r.getTo( ) == Site( ));
		REQUIRE(r.getDistance( ) == Distance( ));
		REQUIRE(r.getDuration( ) == Duration( ));
	}

	SECTION("Copy constructor")
	{
		Route r(make_location(0, 5), make_location(5, 6), Distance(2), Duration(7));

		Route r2(r);
		Route r3 = r;

		REQUIRE(r2 == r);
		REQUIRE(r3 == r2);
	}

	SECTION("Values constructor")
	{
		Route r(make_location(0, 1), make_location(5, 6), Distance(2), Duration(7));

		REQUIRE(r.getFrom( ) == make_location(0, 1));
		REQUIRE(r.getTo( ) == make_location(5, 6));
		REQUIRE(r.getDistance( ) == Distance(2));
		REQUIRE(r.getDuration( ) == Duration(7));
	}

	SECTION("Getters and setters")
	{
		Route r;

		r.setFrom(make_location(4, 7));
		r.setTo(make_location(7, 2));
		r.setDistance(Distance(7));
		r.setDuration(Duration(2));

		REQUIRE(r.getFrom( ) == make_location(4, 7));
		REQUIRE(r.getTo( ) == make_location(7, 2));
		REQUIRE(r.getDistance( ) == Distance(7));
		REQUIRE(r.getDuration( ) == Duration(2));
	}

	SECTION("Comparison operators")
	{
		Route r1(make_location(0, 1), make_location(5, 6), Distance(2), Duration(7));
		Route r2(make_location(0, 5), make_location(6, 2), Distance(1), Duration(8));
		Route r3(make_location(0, 1), make_location(5, 6), Distance(2), Duration(7));

		REQUIRE(r1 == r3);
		REQUIRE(r1 != r2);
	}

	SECTION("Assignment operator")
	{
		Route r1(make_location(0, 1), make_location(5, 6), Distance(2), Duration(7));

		Route r2;

		r2 = r1;

		REQUIRE(r1 == r2);
	}
}