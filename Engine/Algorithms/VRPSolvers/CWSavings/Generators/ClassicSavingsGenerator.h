#pragma once

#include <type_traits>
#include <Engine/SceneManager/Scene.h>
#include "../Savings/ClassicSaving.h"

namespace Scheduler
{
	class ClassicSavingsGenerator
	{
	public:
		template<typename SavingsStore>
		static void generateSavings(Scene* scene, SavingsStore& out_store)
		{
			static_assert(std::is_same<typename SavingsStore::value_type, ClassicSaving>::value, "Incompatible savings store");
		}
	};
}