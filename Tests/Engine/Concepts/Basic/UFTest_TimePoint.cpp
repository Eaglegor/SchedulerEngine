#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <Engine/Concepts/Basic/TimePoint.h>

TEST_CASE("Concepts - Basic - TimePoint", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Object size")
	{
		REQUIRE(sizeof(TimePoint) == sizeof(long));
	}
	
	SECTION("Default constructor")
	{
		TimePoint tp;
		
		REQUIRE(tp.getValue() == 0);
	}
	
	SECTION("Copy constructor")
	{
		TimePoint tp(5);
		
		TimePoint tp2(tp);
		TimePoint tp3 = tp;
		
		REQUIRE(tp.getValue() == tp2.getValue());
		REQUIRE(tp2.getValue() == tp3.getValue());
	}
	
	SECTION("Value constructor")
	{
		TimePoint tp(6);
		
		REQUIRE(tp.getValue() == 6);
	}

	SECTION("Getters and setters")
	{
		TimePoint tp;
		
		tp.setValue(8);
		
		REQUIRE(tp.getValue() == 8);
	}	

	SECTION("Comparison operators")
	{
		TimePoint tp1(1);
		TimePoint tp2(2);
		TimePoint tp3(4);
		TimePoint tp4(1);
		
		REQUIRE(tp1 == tp4);
		REQUIRE(tp1 != tp2);
		
		REQUIRE(tp1 < tp2);
		REQUIRE(tp2 > tp1);
		
		REQUIRE(tp1 <= tp2);
		REQUIRE(tp1 <= tp4);
		REQUIRE(tp2 >= tp1);
		REQUIRE(tp4 >= tp1);
	}

	SECTION("Assignment operator")
	{
		TimePoint tp1(5);
		
		TimePoint tp2;
		
		tp2 = tp1;
		
		REQUIRE(tp1 == tp2);
	}
	
	SECTION("Arithmetic operators")
	{
		TimePoint tp1(1);
		TimePoint tp2(5);
		Duration d(10);
		
		{
			Duration d1 = (tp2 - tp1);
			
			REQUIRE(d1.getValue() == tp2.getValue() - tp1.getValue());
		}
		
		{
			TimePoint tp = (tp1 + d);
			
			REQUIRE(tp.getValue() == tp1.getValue() + d.getValue());
		}
		
		{
			TimePoint tp = (tp1 - d);
			
			REQUIRE(tp.getValue() == tp1.getValue() - d.getValue());
		}
		
		
	}
	
	SECTION("Increment operators")
	{
		TimePoint tp1(1);
		Duration d(5);
		
		tp1+=d;
		tp1+=d;
		
		REQUIRE(tp1.getValue() == 11);
		
		tp1-=d;
		
		REQUIRE(tp1.getValue() == 6);
	}
}