#pragma once

#include <random>
#include <set>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

namespace Scheduler
{

class Run;
class SceneEditor;
class ScheduleCostFunction;

class SolutionGenerator
{
public:
    enum class MutationType : unsigned
    {
        BlockInsert = 1,
        BlockReverse = 2,
        VertexInsert = 4,
        VertexSwap = 8,
        None = 16
    };

    SolutionGenerator (Run* run);

    virtual void neighbour () = 0;

    void enableMutation (MutationType mutation);
    void disableMutation (MutationType mutation);
    void shuffle ();
    void discard ();
    void store ();
    size_t checkpoint ();
    void rollbackTo (size_t checkpoint);

protected:
    typedef std::uniform_int_distribution<size_t> index_distr_t;
    typedef std::uniform_real_distribution<float> float_distr_t;
    typedef index_distr_t::param_type index_param_t;
    typedef float_distr_t::param_type float_param_t;

    void blockInsert(size_t i, size_t j, size_t k);
    void blockReverse(size_t i, size_t j);
    void vertexInsert(size_t i, size_t j);
    void vertexSwap(size_t i, size_t j);
    void printSolution();
    void printSolution(Run* run);

    MutationType selectRandomMutation();

    SceneEditor scene_editor;
    std::mt19937_64 random_engine;
    index_distr_t index_distribution;
    float_distr_t float_distribution;
    ImmutableVector<Run*>::const_iterator run_iter;
    std::set<MutationType> allowed_mutations;
    size_t N;
};

class SolutionGeneratorClassic : public SolutionGenerator
{
public:

    SolutionGeneratorClassic (Run* run);

    virtual void neighbour () override;

private:
    void randomMutation ();
    void blockInsert();
    void blockReverse();
    void vertexInsert();
    void vertexSwap();
};

class InstanceBasedSolutionGenerator : public SolutionGenerator
{
public:
    InstanceBasedSolutionGenerator (Run* run);

    virtual void neighbour () override;
    void setPopulations(std::vector<Run*> populations);

protected:
    void neighbour (Run* anotherRun);
    void neighbour (size_t a, size_t b, bool alternative);
    void addEdgeWithBlockReverse(size_t a, size_t b);
    void addEdgeWithVertexInsert(size_t a, size_t b);
    void addEdgeWithVertexSwap(size_t a, size_t b);
    void addEdgeWithBlockInsert(size_t a, size_t b);

    std::vector<Run*> populations;
};

}
