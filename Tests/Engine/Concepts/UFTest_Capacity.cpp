#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <Engine/Concepts/Capacity.h>

TEST_CASE("Concepts - Basic - Capacity", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Object size")
	{
		REQUIRE(sizeof(Capacity) == sizeof(float) * 4);
	}
	
	SECTION("Default constructor")
	{
		Capacity c;
		
		REQUIRE(c.getValue(0) == Approx(0));
		REQUIRE(c.getValue(1) == Approx(0));
		REQUIRE(c.getValue(2) == Approx(0));
		REQUIRE(c.getValue(3) == Approx(0));
	}

	SECTION("Copy constructor")
	{
		Capacity c(1, 2, 3, 4);
		
		Capacity c2(c);
		Capacity c3 = c;
		
		REQUIRE(c.getValue(0) == Approx(c2.getValue(0)));
		REQUIRE(c.getValue(1) == Approx(c2.getValue(1)));
		REQUIRE(c.getValue(2) == Approx(c2.getValue(2)));
		REQUIRE(c.getValue(3) == Approx(c2.getValue(3)));
		
		REQUIRE(c2.getValue(0) == Approx(c3.getValue(0)));
		REQUIRE(c2.getValue(1) == Approx(c3.getValue(1)));
		REQUIRE(c2.getValue(2) == Approx(c3.getValue(2)));
		REQUIRE(c2.getValue(3) == Approx(c3.getValue(3)));
	}
	
	SECTION("Values constructor")
	{
		Capacity c(1, 2, 3 ,4);
		
		REQUIRE(c.getValue(0) == Approx(1));
		REQUIRE(c.getValue(1) == Approx(2));
		REQUIRE(c.getValue(2) == Approx(3));
		REQUIRE(c.getValue(3) == Approx(4));
	}
	
	SECTION("Getters and setters")
	{
		Capacity c;
		
		c.setValue(0, 1);
		c.setValue(1, 2);
		c.setValue(2, 3);
		c.setValue(3, 4);
		
		REQUIRE(c.getValue(0) == Approx(1));
		REQUIRE(c.getValue(1) == Approx(2));
		REQUIRE(c.getValue(2) == Approx(3));
		REQUIRE(c.getValue(3) == Approx(4));
		
		c.setValues(5, 6, 7, 8);
		
		REQUIRE(c.getValue(0) == Approx(5));
		REQUIRE(c.getValue(1) == Approx(6));
		REQUIRE(c.getValue(2) == Approx(7));
		REQUIRE(c.getValue(3) == Approx(8));
	}	
	
	SECTION("Indexing operator")
	{
		Capacity c;
		
		c[0] = 1;
		c[1] = 2;
		c[2] = 3;
		c[3] = 4;
		
		REQUIRE(c[0] == Approx(1));
		REQUIRE(c[1] == Approx(2));
		REQUIRE(c[2] == Approx(3));
		REQUIRE(c[3] == Approx(4));
	}	
	

	SECTION("Comparison operators")
	{
		Capacity c1(1, 2, 3, 4);
		Capacity c2(5, 6, 7, 8);
		Capacity c3(4, 3, 2, 1);
		Capacity c4(1, 2, 3, 4);
		
		REQUIRE(c1 == c4);
		REQUIRE(c1 != c2);
		
		REQUIRE(c1 < c2);
		REQUIRE(c2 > c1);
		
		REQUIRE( !(c1 < c3 || c3 < c1) );
		REQUIRE( !(c1 > c3 || c3 > c1) );
		
		REQUIRE(c1 <= c2);
		REQUIRE(c1 <= c4);
		REQUIRE(c2 >= c1);
		REQUIRE(c4 >= c1);
		
		REQUIRE( !(c1 <= c3 || c3 <= c1) );
		REQUIRE( !(c1 >= c3 || c3 >= c1) );
	}

	SECTION("Assignment operator")
	{
		Capacity c1(1, 2, 3, 4);
		
		Capacity c2;
		
		c2 = c1;
		
		REQUIRE(c1 == c2);
	}
	
	SECTION("Arithmetic operators")
	{
		Capacity c1(1, 2, 3, 4);
		Capacity c2(5, 6, 7, 8);
		
		{
			Capacity c3 = (c1 + c2);
			
			REQUIRE(c3.getValue(0) == Approx(c1.getValue(0) + c2.getValue(0)));
			REQUIRE(c3.getValue(1) == Approx(c1.getValue(1) + c2.getValue(1)));
			REQUIRE(c3.getValue(2) == Approx(c1.getValue(2) + c2.getValue(2)));
			REQUIRE(c3.getValue(3) == Approx(c1.getValue(3) + c2.getValue(3)));
		}		
		
		{
			Capacity c3 = (c1 - c2);
			
			REQUIRE(c3.getValue(0) == Approx(c1.getValue(0) - c2.getValue(0)));
			REQUIRE(c3.getValue(1) == Approx(c1.getValue(1) - c2.getValue(1)));
			REQUIRE(c3.getValue(2) == Approx(c1.getValue(2) - c2.getValue(2)));
			REQUIRE(c3.getValue(3) == Approx(c1.getValue(3) - c2.getValue(3)));
		}		
		
		{
			Capacity c3 = -c1;
			
			REQUIRE(c3.getValue(0) == Approx(-(c1.getValue(0))));
			REQUIRE(c3.getValue(1) == Approx(-(c1.getValue(1))));
			REQUIRE(c3.getValue(2) == Approx(-(c1.getValue(2))));
			REQUIRE(c3.getValue(3) == Approx(-(c1.getValue(3))));
		}		
	}
	
	SECTION("Increment operators")
	{
		Capacity c1(1, 2, 3, 4);
		Capacity c2(5, 6, 7, 8);
		
		c1+=c2;
		c1+=c2;
		
		REQUIRE(c1.getValue(0) == Approx(11));
		REQUIRE(c1.getValue(1) == Approx(14));
		REQUIRE(c1.getValue(2) == Approx(17));
		REQUIRE(c1.getValue(3) == Approx(20));
		
		c1-=c2;
		
		REQUIRE(c1.getValue(0) == Approx(6));
		REQUIRE(c1.getValue(1) == Approx(8));
		REQUIRE(c1.getValue(2) == Approx(10));
		REQUIRE(c1.getValue(3) == Approx(12));
	}
	
	
	
}