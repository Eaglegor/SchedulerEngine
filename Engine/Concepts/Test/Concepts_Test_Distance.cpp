#include <catch.hpp>

#include <Engine/Concepts/Distance.h>

TEST_CASE("Concepts - Basic - Distance", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Object size")
	{
		REQUIRE(sizeof(Distance) == sizeof(float));
	}
	
	SECTION("Default constructor")
	{
		Distance d;
		
		REQUIRE(d.getValue() == Approx(0));
	}
	
	SECTION("Copy constructor")
	{
		Distance d(5);
		
		Distance d2(d);
		Distance d3 = d;
		
		REQUIRE(d.getValue() == Approx(d2.getValue()));
		REQUIRE(d2.getValue() == Approx(d3.getValue()));
	}
	
	SECTION("Value constructor")
	{
		Distance d(6);
		
		REQUIRE(d.getValue() == Approx(6));
	}

	SECTION("Getters and setters")
	{
		Distance d;
		
		d.setValue(8);
		
		REQUIRE(d.getValue() == Approx(8));
	}	

	SECTION("Comparison operators")
	{
		Distance d1(1);
		Distance d2(2);
		Distance d3(4);
		Distance d4(1);
		
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
		Distance d1(5);
		
		Distance d2;
		
		d2 = d1;
		
		REQUIRE(d1 == d2);
	}
	
	SECTION("Arithmetic operators")
	{
		Distance d1(1);
		Distance d2(5);
		
		{
			Distance d3 = (d1 + d2);
			
			REQUIRE(d3.getValue() == Approx(d1.getValue() + d2.getValue()));
		}
		
		{
			Distance d3 = (d1 - d2);
			
			REQUIRE(d3.getValue() == Approx(d1.getValue() - d2.getValue()));
		}
		
		{
			Distance d3 = -d1;
			
			REQUIRE(d3.getValue() == Approx(-d1.getValue()));
		}
		
		{
			Distance d3 = d1 * d2.getValue();
			
			REQUIRE(d3.getValue() == Approx(d1.getValue() * d2.getValue()));
		}
		
		{
			Distance d3 = d1 / d2.getValue();
			
			REQUIRE(d3.getValue() == Approx(d1.getValue() / d2.getValue()));
		}
	}
	
	SECTION("Increment operators")
	{
		Distance d1(1);
		Distance d2(5);
		
		d1+=d2;
		d1+=d2;
		
		REQUIRE(d1.getValue() == Approx(11));
		
		d1-=d2;
		
		REQUIRE(d1.getValue() == Approx(6));
		
		d1*=2.0f;
		
		REQUIRE(d1.getValue() == Approx(12));
		
		d1/=2.0f;
		
		REQUIRE(d1.getValue() == Approx(6));
	}
	
	
	
}