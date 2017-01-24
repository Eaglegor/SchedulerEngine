#include "SimpleEAVRPSolver.h"

namespace Scheduler
{
    void SimpleEAVRPSolver::optimize(Scene& scene)
    {
        
    }

    void SimpleEAVRPSolver::setCostFunction(const CostFunction& cost_function)
    {
        this->cost_function = cost_function;
    }
}