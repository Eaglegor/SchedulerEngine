#include <catch.hpp>

#include <Engine/Utils/Collections/Algorithms.h>

#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

TEST_CASE("Utils - Collections - Algorithms", "[unit][functional][utils]")
{
	SECTION("std::contains_key")
	{
		std::set<int> s{0, 1, 2};
		std::unordered_set<int> us{0, 1, 2};

		std::map<int, int> m{{0, 0},
		                     {1, 1},
		                     {2, 2}};

		std::unordered_map<int, int> um{{0, 0},
		                                {1, 1},
		                                {2, 2}};

		REQUIRE(util::contains_key(s, 1));
		REQUIRE(util::contains_key(us, 1));
		REQUIRE(util::contains_key(m, 1));
		REQUIRE(util::contains_key(um, 1));

		REQUIRE_FALSE(util::contains_key(s, 5));
		REQUIRE_FALSE(util::contains_key(us, 5));
		REQUIRE_FALSE(util::contains_key(m, 5));
		REQUIRE_FALSE(util::contains_key(um, 5));
	}

	SECTION("std::contains")
	{
		std::vector<int> v{0, 1, 2};

		std::list<int> l{0, 1, 2};

		REQUIRE(util::contains(v, 1));
		REQUIRE(util::contains(l, 1));

		REQUIRE_FALSE(util::contains(v, 5));
		REQUIRE_FALSE(util::contains(l, 5));
	}
}
