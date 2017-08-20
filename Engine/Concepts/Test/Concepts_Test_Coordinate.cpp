#include <catch.hpp>

#include <Engine/Concepts/Coordinate.h>

TEST_CASE("Concepts - Basic - Coordinate", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Object size")
	{
		REQUIRE(sizeof(Coordinate) == sizeof(float));
	}

	SECTION("Default constructor")
	{
		Coordinate c;

		REQUIRE(c.getValue( ) == Approx(0));
	}

	SECTION("Copy constructor")
	{
		Coordinate c(5);

		Coordinate c2(c);
		Coordinate c3 = c;

		REQUIRE(c.getValue( ) == Approx(c2.getValue( )));
		REQUIRE(c2.getValue( ) == Approx(c3.getValue( )));
	}

	SECTION("Value constructor")
	{
		Coordinate c(6);

		REQUIRE(c.getValue( ) == Approx(6));
	}

	SECTION("Getters and setters")
	{
		Coordinate c;

		c.setValue(8);

		REQUIRE(c.getValue( ) == Approx(8));
	}

	SECTION("Comparison operators")
	{
		Coordinate c1(1);
		Coordinate c2(2);
		Coordinate c3(4);
		Coordinate c4(1);

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
		Coordinate c1(5);

		Coordinate c2;

		c2 = c1;

		REQUIRE(c1 == c2);
	}

	SECTION("Arithmetic operators")
	{
		Coordinate c1(1);
		Coordinate c2(5);

		{
			Coordinate c3 = (c1 + c2);

			REQUIRE(c3.getValue( ) == Approx(c1.getValue( ) + c2.getValue( )));
		}

		{
			Coordinate c3 = (c1 - c2);

			REQUIRE(c3.getValue( ) == Approx(c1.getValue( ) - c2.getValue( )));
		}

		{
			Coordinate c3 = -c1;

			REQUIRE(c3.getValue( ) == Approx(-c1.getValue( )));
		}

		{
			Coordinate c3 = c1 * c2.getValue( );

			REQUIRE(c3.getValue( ) == Approx(c1.getValue( ) * c2.getValue( )));
		}

		{
			Coordinate c3 = c1 / c2.getValue( );

			REQUIRE(c3.getValue( ) == Approx(c1.getValue( ) / c2.getValue( )));
		}
	}

	SECTION("Increment operators")
	{
		Coordinate c1(1);
		Coordinate c2(5);

		c1 += c2;
		c1 += c2;

		REQUIRE(c1.getValue( ) == Approx(11));

		c1 -= c2;

		REQUIRE(c1.getValue( ) == Approx(6));

		c1 *= 2.0f;

		REQUIRE(c1.getValue( ) == Approx(12));

		c1 /= 2.0f;

		REQUIRE(c1.getValue( ) == Approx(6));
	}
}