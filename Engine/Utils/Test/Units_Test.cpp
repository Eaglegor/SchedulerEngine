#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <Engine/Utils/Units/DurationUnits.h>

using namespace Scheduler;

TEST_CASE("Simple case") {
	Scheduler::Duration sec = Units::seconds(5);
	REQUIRE_FALSE(sec.getValue() == 0);
}