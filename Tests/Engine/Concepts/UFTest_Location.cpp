#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <Engine/Concepts/Location.h>

TEST_CASE("Concepts - Basic - Location", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Default constructor")
	{
		Location l;
		
		REQUIRE(l.getLatitude() == Coordinate(0));
		REQUIRE(l.getLongitude() == Coordinate(0));
	}
	
	SECTION("Copy constructor")
	{
		Location l(Coordinate(5), Coordinate(5));
		
		Location l2(l);
		Location l3 = l;
		
		REQUIRE(l.getLatitude() == l2.getLatitude());
		REQUIRE(l.getLongitude() == l2.getLongitude());
		
		REQUIRE(l2.getLatitude() == l3.getLatitude());
		REQUIRE(l2.getLongitude() == l3.getLongitude());
	}
	
	SECTION("Values constructor")
	{
		Location l(Coordinate(1), Coordinate(5));
		
		REQUIRE(l.getLatitude() == Coordinate(1));
		REQUIRE(l.getLongitude() == Coordinate(5));
	}

	SECTION("Getters and setters")
	{
		Location l;
		
		l.setLatitude(Coordinate(1));
		l.setLongitude(Coordinate(5));
		
		REQUIRE(l.getLatitude() == Coordinate(1));
		REQUIRE(l.getLongitude() == Coordinate(5));
	}

	SECTION("Comparison operators")
	{
		Location l1(Coordinate(1), Coordinate(2));
		Location l2(Coordinate(4), Coordinate(3));
		Location l3(Coordinate(1), Coordinate(2));
		
		REQUIRE(l1 == l3);
		REQUIRE(l1 != l2);
	}

	SECTION("Assignment operator")
	{
		Location l1(Coordinate(4), Coordinate(6));
		
		Location l2;
		
		l2 = l1;
		
		REQUIRE(l1 == l2);
	}

}