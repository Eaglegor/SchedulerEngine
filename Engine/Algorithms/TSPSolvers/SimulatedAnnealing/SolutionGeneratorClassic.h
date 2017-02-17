#pragma once

#include "SolutionGenerator.h"

namespace Scheduler
{

class SolutionGeneratorClassic : public SolutionGenerator
{
public:

    explicit SolutionGeneratorClassic(Run& run);

    virtual void neighbour() override;

private:
    void randomMutation();
    void blockInsert();
    void blockReverse();
    void vertexInsert();
    void vertexSwap();
};

}
