#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <Engine/Concepts/Basic/Duration.h>

TEST_CASE("Concepts - Basic - Duration", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Object size")
	{
		REQUIRE(sizeof(Duration) == sizeof(long));
	}
	
	SECTION("Default constructor")
	{
		Duration d;
		
		REQUIRE(d.getValue() == 0);
	}
	
	SECTION("Copy constructor")
	{
		Duration d(5);
		
		Duration d2(d);
		Duration d3 = d;
		
		REQUIRE(d.getValue() == d2.getValue());
		REQUIRE(d2.getValue() == d3.getValue());
	}
	
	SECTION("Value constructor")
	{
		Duration d(6);
		
		REQUIRE(d.getValue() == 6);
	}

	SECTION("Getters and setters")
	{
		Duration d;
		
		d.setValue(8);
		
		REQUIRE(d.getValue() == 8);
	}	

	SECTION("Comparison operators")
	{
		Duration d1(1);
		Duration d2(2);
		Duration d3(4);
		Duration d4(1);
		
		REQUIRE(d1 == d4);
		REQUIRE(d1 != d2);
		
		REQUIRE(d1 < d2);
		REQUIRE(d2 > d1);
		
		REQUIRE(d1 <= d2);
		REQUIRE(d1 <= d4);
		REQUIRE(d2 >= d1);
		REQUIRE(d4 >= d1);
	}

	SECTION("Assignment operator")
	{
		Duration d1(5);
		
		Duration d2;
		
		d2 = d1;
		
		REQUIRE(d1 == d2);
	}
	
	SECTION("Arithmetic operators")
	{
		Duration d1(1);
		Duration d2(5);
		
		{
			Duration d3 = (d1 + d2);
			
			REQUIRE(d3.getValue() == d1.getValue() + d2.getValue());
		}
		
		{
			Duration d3 = (d1 - d2);
			
			REQUIRE(d3.getValue() == d1.getValue() - d2.getValue());
		}
		
		{
			Duration d3 = -d1;
			
			REQUIRE(d3.getValue() == -d1.getValue());
		}
		
		{
			Duration d3 = d1 * d2.getValue();
			
			REQUIRE(d3.getValue() == d1.getValue() * d2.getValue());
		}
		
		{
			Duration d3 = d1 / d2.getValue();
			
			REQUIRE(d3.getValue() == d1.getValue() / d2.getValue());
		}
	}
	
	SECTION("Increment operators")
	{
		Duration d1(1);
		Duration d2(5);
		
		d1+=d2;
		d1+=d2;
		
		REQUIRE(d1.getValue() == 11);
		
		d1-=d2;
		
		REQUIRE(d1.getValue() == 6);
		
		d1*=2;
		
		REQUIRE(d1.getValue() == 12);
		
		d1/=2;
		
		REQUIRE(d1.getValue() == 6);
	}
	
	
	
}