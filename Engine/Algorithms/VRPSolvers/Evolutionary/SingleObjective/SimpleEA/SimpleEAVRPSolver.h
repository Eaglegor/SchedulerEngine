#pragma once

#include <Engine/AlgorithmsManager/VRPSolver.h>
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <vector>

#include <SimpleEAVRPSolver_export.h>

namespace Scheduler
{
    class SceneCostFunction;
    class Scene;

	
    class SIMPLEEAVRPSOLVER_EXPORT SimpleEAVRPSolver : public VRPSolver
    {
        public:
            void optimize(Scene& scene) const override;

            static const char* staticGetName() {return "SimpleEA";}
            const char* getName() const override{ return staticGetName();}

            void setCostFunction(const SceneCostFunction& cost_function);
			void setIterationsLimit(std::size_t iterations_limit);
			void setPopulationSize(std::size_t population_size);
			
        private:
			
			
            Optional<const SceneCostFunction&> cost_function;
			std::size_t iterations_limit = 100;
			std::size_t population_size = 20;
    };
}