#include "SimpleEAVRPSolver.h"
#include <Engine/Utils/ReferenceWrapper.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/AlgorithmsManager/CostFunction.h>

namespace Scheduler
{
    void SimpleEAVRPSolver::optimize(Scene& scene) const
    {
		std::vector<ReferenceWrapper<Scene>> population;
		
		// Random solution
		
		const SceneContext& context = scene.getContext();
		
		for(int i = 0; i < population_size; ++i)
		{
			
		}
		
		// Evolution 
		
		bool finished = false;
		std::size_t iterations_count = 0;
		
		while(!finished)
		{
			
			
			if(++iterations_count >= iterations_limit) finished = true;
		}
    }

    void SimpleEAVRPSolver::setCostFunction(const CostFunction& cost_function)
    {
        this->cost_function = cost_function;
    }
    
	void SimpleEAVRPSolver::setIterationsLimit(std::size_t iterations_limit)
	{
		this->iterations_limit = iterations_limit;
	}

}