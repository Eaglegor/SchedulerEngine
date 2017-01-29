#pragma once

#include <Engine/AlgorithmsManager/VRPSolver.h>
#include <Engine/Utils/Optional.h>

namespace Scheduler
{
    class CostFunction;
    class Scene;

    class SimpleEAVRPSolver : public VRPSolver
    {
        public:
            void optimize(Scene& scene) const override;

            static const char* staticGetName() {return "SimpleEA";}
            const char* getName() const override{ return staticGetName();}

            void setCostFunction(const CostFunction& cost_function);
			void setIterationsLimit(std::size_t iterations_limit);
			void setPopulationSize(std::size_t population_size);
			
        private:
            Optional<const CostFunction&> cost_function;
			std::size_t iterations_limit = 100;
			std::size_t population_size = 20;
    };
}