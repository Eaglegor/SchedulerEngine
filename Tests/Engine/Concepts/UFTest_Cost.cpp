#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <Engine/Concepts/Cost.h>

#include <iostream>

TEST_CASE("Concepts - Basic - Cost", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Object size")
	{
		REQUIRE(sizeof(Cost) == sizeof(float));
	}

	SECTION("Default constructor")
	{
		Cost c;
		
		REQUIRE(c.getValue() == Approx(0));
	}
	
	SECTION("Copy constructor")
	{
		Cost c(5);
		
		Cost c2(c);
		Cost c3 = c;
		
		REQUIRE(c.getValue() == Approx(c2.getValue()));
		REQUIRE(c2.getValue() == Approx(c3.getValue()));
	}
	
	SECTION("Value constructor")
	{
		Cost c(6);
		
		REQUIRE(c.getValue() == Approx(6));
	}

	SECTION("Getters and setters")
	{
		Cost c;
		
		c.setValue(8);
		
		REQUIRE(c.getValue() == Approx(8));
	}	

	SECTION("Comparison operators")
	{
		Cost c1(1);
		Cost c2(2);
		Cost c3(4);
		Cost c4(1);
		
		REQUIRE(c1 == c4);
		REQUIRE(c1 != c2);
		
		REQUIRE(c1 < c2);
		REQUIRE(c2 > c1);
		
		REQUIRE(c1 <= c2);
		REQUIRE(c1 <= c4);
		REQUIRE(c2 >= c1);
		REQUIRE(c4 >= c1);
	}

	SECTION("Assignment operator")
	{
		Cost c1(5);
		
		Cost c2;
		
		c2 = c1;
		
		REQUIRE(c1 == c2);
	}
	
	SECTION("Arithmetic operators")
	{
		Cost c1(1);
		Cost c2(5);
		
		{
			Cost c3 = (c1 + c2);
			
			REQUIRE(c3.getValue() == Approx(c1.getValue() + c2.getValue()));
		}
		
		{
			Cost c3 = (c1 - c2);
			
			REQUIRE(c3.getValue() == Approx(c1.getValue() - c2.getValue()));
		}
		
		{
			Cost c3 = -c1;
			
			REQUIRE(c3.getValue() == Approx(-c1.getValue()));
		}
		
		{
			Cost c3 = c1 * c2.getValue();
			
			REQUIRE(c3.getValue() == Approx(c1.getValue() * c2.getValue()));
		}
		
		{
			Cost c3 = c1 / c2.getValue();
			
			REQUIRE(c3.getValue() == Approx(c1.getValue() / c2.getValue()));
		}
	}
	
	SECTION("Increment operators")
	{
		Cost c1(1);
		Cost c2(5);
		
		c1+=c2;
		c1+=c2;
		
		REQUIRE(c1.getValue() == Approx(11));
		
		c1-=c2;
		
		REQUIRE(c1.getValue() == Approx(6));
		
		c1*=2.0f;
		
		REQUIRE(c1.getValue() == Approx(12));
		
		c1/=2.0f;
		
		REQUIRE(c1.getValue() == Approx(6));
	}
	
	
	
}