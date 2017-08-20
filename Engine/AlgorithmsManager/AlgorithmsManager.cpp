#include "AlgorithmsManager.h"
#include <Engine/MemoryManager/ObjectSizes.h>

namespace Scheduler
{
	AlgorithmsManager::AlgorithmsManager( )
	    : algorithms_factory(Pool::LARGE_OBJECT, 10),
	      cost_functions_factory(Pool::LARGE_OBJECT, 10)
	{
	}

	AlgorithmsManager::~AlgorithmsManager( )
	{
		for(Algorithm& algorithm : algorithms)
		{
			algorithms_factory.destroyObject(const_cast<Algorithm*>(&algorithm));
		}
		for(CostFunction& cf : cost_functions)
		{
			cost_functions_factory.destroyObject(const_cast<CostFunction*>(&cf));
		}
	}

	void AlgorithmsManager::destroyAlgorithm(Algorithm& algorithm)
	{
		algorithms.erase(algorithm);
		algorithms_factory.destroyObject(&algorithm);
	}

	void AlgorithmsManager::destroyCostFunction(CostFunction& cf)
	{
		cost_functions.erase(cf);
		cost_functions_factory.destroyObject(&cf);
	}
}
