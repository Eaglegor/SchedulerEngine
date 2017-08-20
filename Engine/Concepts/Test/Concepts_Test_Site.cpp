#include <catch.hpp>

#include <Engine/Concepts/Site.h>

TEST_CASE("Concepts - Basic - Site", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Default constructor")
	{
		Site l;

		REQUIRE(l.getLatitude( ) == Coordinate(0));
		REQUIRE(l.getLongitude( ) == Coordinate(0));
	}

	SECTION("Copy constructor")
	{
		Site l(Coordinate(5), Coordinate(5));

		Site l2(l);
		Site l3 = l;

		REQUIRE(l.getLatitude( ) == l2.getLatitude( ));
		REQUIRE(l.getLongitude( ) == l2.getLongitude( ));

		REQUIRE(l2.getLatitude( ) == l3.getLatitude( ));
		REQUIRE(l2.getLongitude( ) == l3.getLongitude( ));
	}

	SECTION("Values constructor")
	{
		Site l(Coordinate(1), Coordinate(5));

		REQUIRE(l.getLatitude( ) == Coordinate(1));
		REQUIRE(l.getLongitude( ) == Coordinate(5));
	}

	SECTION("Getters and setters")
	{
		Site l;

		l.setLatitude(Coordinate(1));
		l.setLongitude(Coordinate(5));

		REQUIRE(l.getLatitude( ) == Coordinate(1));
		REQUIRE(l.getLongitude( ) == Coordinate(5));
	}

	SECTION("Comparison operators")
	{
		Site l1(Coordinate(1), Coordinate(2));
		Site l2(Coordinate(4), Coordinate(3));
		Site l3(Coordinate(1), Coordinate(2));

		REQUIRE(l1 == l3);
		REQUIRE(l1 != l2);
	}

	SECTION("Assignment operator")
	{
		Site l1(Coordinate(4), Coordinate(6));

		Site l2;

		l2 = l1;

		REQUIRE(l1 == l2);
	}
}