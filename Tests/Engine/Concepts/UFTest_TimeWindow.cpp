#include <sch_test>
#include <sch_test_utils>
#include <Engine/Concepts/TimeWindow.h>

TEST_CASE("Concepts - Basic - TimeWindow", "[unit][functional][concepts]")
{
	using namespace Scheduler;

	SECTION("Default constructor")
	{
		TimeWindow tw;

		REQUIRE(tw.getStartTime() == TimePoint::min());
		REQUIRE(tw.getEndTime() == TimePoint::max());
		REQUIRE(tw.getDuration() == Duration::max());
	}

	SECTION("Copy constructor")
	{
		TimeWindow tw(TimePoint(10), TimePoint(50));

		TimeWindow tw2(tw);
		TimeWindow tw3 = tw2;

		REQUIRE(tw.getStartTime() == tw2.getStartTime());
		REQUIRE(tw.getEndTime() == tw2.getEndTime());
		REQUIRE(tw.getDuration() == tw2.getDuration());

		REQUIRE(tw2.getStartTime() == tw3.getStartTime());
		REQUIRE(tw2.getEndTime() == tw3.getEndTime());
		REQUIRE(tw2.getDuration() == tw3.getDuration());
	}

	SECTION("Value constructor")
	{
		TimeWindow tw = make_time_window(10, 50);

		REQUIRE(tw.getStartTime() == TimePoint(10));
		REQUIRE(tw.getEndTime() == TimePoint(50));
		REQUIRE(tw.getDuration() == Duration(40));
	}

	SECTION("Getters and setters")
	{
		TimeWindow tw;

		tw.setStartTime(TimePoint(5));
		tw.setEndTime(TimePoint(20));

		REQUIRE(tw.getStartTime() == TimePoint(5));
		REQUIRE(tw.getEndTime() == TimePoint(20));
		REQUIRE(tw.getDuration() == Duration(15));
	}

	SECTION("Comparison operators")
	{
		TimeWindow tw1 = make_time_window(10, 60);
		TimeWindow tw2 = make_time_window(45, 60);
		TimeWindow tw3 = make_time_window(10, 60);

		REQUIRE(tw1 == tw3);
		REQUIRE(tw1 != tw2);
	}

	SECTION("Assignment operator")
	{
		TimeWindow tw1 = make_time_window(30, 45);

		TimeWindow tw2;

		tw2 = tw1;

		REQUIRE(tw1 == tw2);
	}
}