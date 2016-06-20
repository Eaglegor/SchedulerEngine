#include <catch.hpp>

#include <Engine/Utils/Units/SpeedUnits.h>

TEST_CASE("Concepts - Basic - SpeedUnits", "[unit][functional][concepts]")
{
    using namespace Scheduler;

    SECTION("Speed per hour (Distance)")
    {
        REQUIRE(Units::speedPerHour(Distance(60)) == Speed(Distance(0.01666666666666667)));
    }

    SECTION("Speed per hour (float)")
    {
        REQUIRE(Units::speedPerHour(60.0f) == Speed(Distance(0.01666666666666667)));
    }
}