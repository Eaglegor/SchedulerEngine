#include <catch.hpp>

#include <Engine/Concepts/Speed.h>

TEST_CASE("Concepts - Basic - Speed", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Object size")
	{
		REQUIRE(sizeof(Speed) == sizeof(Distance));
	}

	SECTION("Default constructor")
	{
		Speed s;

		REQUIRE(s.getValue() == Distance(0));
	}

	SECTION("Copy constructor")
	{
		Speed s(Distance(5));

		Speed s2(s);
		Speed s3 = s;

		REQUIRE(s.getValue() == s2.getValue());
		REQUIRE(s2.getValue() == s3.getValue());
	}

	SECTION("Value constructor")
	{
		Speed s(Distance(6));

		REQUIRE(s.getValue() == Distance(6));
	}

	SECTION("Getters and setters")
	{
		Speed s;

		s.setValue(Distance(8));

		REQUIRE(s.getValue() == Distance(8));
	}

	SECTION("Comparison operators")
	{
		Speed s1(Distance(1));
		Speed s2(Distance(2));
		Speed s3(Distance(3));
		Speed s4(Distance(1));

		REQUIRE(s1 == s4);
		REQUIRE(s1 != s2);

		REQUIRE(s1 < s2);
		REQUIRE(s2 > s1);

		REQUIRE(s1 <= s2);
		REQUIRE(s1 <= s4);
		REQUIRE(s2 >= s1);
		REQUIRE(s4 >= s1);
	}

	SECTION("Assignment operator")
	{
		Speed s1(Distance(5));

		Speed s2;

		s2 = s1;

		REQUIRE(s1 == s2);
	}

	SECTION("Arithmetic operators")
	{
		Speed s1(Distance(1));
		Speed s2(Distance(5));

		{
			Speed s3 = (s1 + s2);

			REQUIRE(s3.getValue() == s1.getValue() + s2.getValue());
		}

		{
			Speed s3 = (s1 - s2);

			REQUIRE(s3.getValue() == s1.getValue() - s2.getValue());
		}

		{
			Speed s3 = -s1;

			REQUIRE(s3.getValue() == -s1.getValue());
		}

		{
			Speed s3 = s1 * s2.getValue().getValue();

			REQUIRE(s3.getValue() == s1.getValue() * s2.getValue().getValue());
		}

		{
			Speed s3 = s1 / s2.getValue().getValue();

			REQUIRE(s3.getValue() == s1.getValue() / s2.getValue().getValue());
		}
	}

	SECTION("Increment operators")
	{
		Speed s1(Distance(1));
		Speed s2(Distance(5));

		s1+=s2;
		s1+=s2;

		REQUIRE(s1.getValue() == Distance(11));

		s1-=s2;

		REQUIRE(s1.getValue() == Distance(6));

		s1*=2.0f;

		REQUIRE(s1.getValue() == Distance(12));

		s1/=2.0f;

		REQUIRE(s1.getValue() == Distance(6));
	}



}