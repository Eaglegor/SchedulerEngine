#pragma once

#include "Algorithm.h"
#include "CostFunction.h"
#include <Engine/Utils/Factory.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <functional>
#include <type_traits>
#include <unordered_set>



namespace Scheduler
{
	class Strategy;

	/** 
	 * @brief This class created the algorithms and cost functions and manages their lifetimes
	 */
	class AlgorithmsManager
	{
	public:
		/** 
		 * @brief Default constructor
		 */
		AlgorithmsManager( );
		explicit AlgorithmsManager(const AlgorithmsManager&) = delete;
		~AlgorithmsManager( );

		/** 
		 * @brief Creates new algorithm
		 * 
		 * @details Algorithm is created using the underlying factory and the reference to the newly created instance is returned.
		 * The lifetime of created algorithms is managed by AlgorithmsManager. Algorithms that weren't explicitly destroyed
		 * by the call to the destroyAlgorithm method will be automatically destroyed when the AlgorithmsManager is destroyed.
		 * 
		 * @tparam Alg Algorithm type to be created
		 * @tparam Args Argument types that will be passed to the algorithm constructor
		 * @param args Arguments to be passed to the algorithm constructor
		 * @return Reference to the instantiated algorithm
		 */
		template <typename Alg, typename... Args>
		Alg& createAlgorithm(Args&&... args)
		{
			static_assert(std::is_base_of<Algorithm, Alg>::value, "Invalid algorithm type passed: expected class derived from Scheduler::Algorithm");
			Alg& alg = *algorithms_factory.createObject<Alg>(std::forward<Args>(args)...);
			algorithms.emplace(alg);
			return alg;
		}

		/** 
		 * @brief Creates new cost function
		 * 
		 * @details Cost function is created using the underlying factory and the reference to the newly created instance is returned.
		 * The lifetime of created cost functions is managed by AlgorithmsManager. Cost functions that weren't explicitly destroyed
		 * by the call to the destroyCostFunction method will be automatically destroyed when the AlgorithmsManager is destroyed.
		 * 
		 * @tparam Cf Cost function type to be created
		 * @tparam Args Argument types that will be passed to the cost function constructor
		 * @param args Arguments to be passed to the cost function constructor
		 * @return Reference to the instantiated cost function
		 */
		template <typename Cf, typename... Args>
		Cf& createCostFunction(Args&&... args)
		{
			static_assert(std::is_base_of<CostFunction, Cf>::value, "Invalid cost function type passed: expected class derived from Scheduler::CostFunction");
			Cf& cf = *cost_functions_factory.createObject<Cf>(std::forward<Args>(args)...);
			cost_functions.emplace(cf);
			return cf;
		}

		/**
		 * @brief Destroys algorithm
		 * 
		 * @details The destructor will be called on the passed algorithm and then the allocated memory will be freed.
		 * 
		 * @note Passing the algorithm that wasn't created by this algorithms manager causes an undefined behavior
		 * 
		 * @param algorithm Algorithm to be destroyed
		 */
		void destroyAlgorithm(Algorithm& algorithm);

		/**
		 * @brief Destroys cost function
		 * 
		 * @details The destructor will be called on the passed cost function and then the allocated memory will be freed.
		 * 
		 * @note Passing the cost function that wasn't created by this algorithms manager causes an undefined behavior
		 * 
		 * @param cf Cost function to be destroyed
		 */
		void destroyCostFunction(CostFunction& cf);

	private:
		Factory<Algorithm> algorithms_factory;
		Factory<CostFunction> cost_functions_factory;

		std::unordered_set<ReferenceWrapper<Algorithm>> algorithms;
		std::unordered_set<ReferenceWrapper<CostFunction>> cost_functions;
	};
}