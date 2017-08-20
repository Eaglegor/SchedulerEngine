#pragma once

#include "Algorithm.h"
#include "CostFunction.h"
#include <Engine/Utils/Factory.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <functional>
#include <type_traits>
#include <unordered_set>

#include <AlgorithmsManager_export.h>

namespace Scheduler
{
	class Strategy;

	class ALGORITHMSMANAGER_EXPORT AlgorithmsManager
	{
	public:
		AlgorithmsManager( );
		explicit AlgorithmsManager(const AlgorithmsManager&) = delete;
		~AlgorithmsManager( );

		template <typename Alg, typename... Args>
		Alg& createAlgorithm(Args&&... args)
		{
			static_assert(std::is_base_of<Algorithm, Alg>::value, "Invalid algorithm type passed: expected class derived from Scheduler::Algorithm");
			Alg& alg = *algorithms_factory.createObject<Alg>(std::forward<Args>(args)...);
			algorithms.emplace(alg);
			return alg;
		}

		template <typename Cf, typename... Args>
		Cf& createCostFunction(Args&&... args)
		{
			static_assert(std::is_base_of<CostFunction, Cf>::value, "Invalid cost function type passed: expected class derived from Scheduler::CostFunction");
			Cf& cf = *cost_functions_factory.createObject<Cf>(std::forward<Args>(args)...);
			cost_functions.emplace(cf);
			return cf;
		}

		void destroyAlgorithm(Algorithm& algorithm);
		void destroyCostFunction(CostFunction& cf);

	private:
		Factory<Algorithm> algorithms_factory;
		Factory<CostFunction> cost_functions_factory;

		std::unordered_set<ReferenceWrapper<Algorithm>> algorithms;
		std::unordered_set<ReferenceWrapper<CostFunction>> cost_functions;
	};
}