#pragma once

#include <random>
#include <set>
#include <unordered_map>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <Engine/SceneManager/Run.h>

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

    explicit SolutionGenerator (Run& run);

    virtual void neighbour () = 0;

    void enableMutation (MutationType mutation);
    void disableMutation (MutationType mutation);
    void discard ();
    void store ();
    std::size_t checkpoint ();
    void rollbackTo (std::size_t checkpoint);
    bool hasPermutation () const;

protected:
    typedef std::uniform_int_distribution<std::size_t> index_distr_t;
    typedef std::uniform_real_distribution<float> float_distr_t;
    typedef index_distr_t::param_type index_param_t;
    typedef float_distr_t::param_type float_param_t;

    void blockInsert(std::size_t i, std::size_t j, std::size_t k);
    void blockReverse(std::size_t i, std::size_t j);
    void vertexInsert(std::size_t i, std::size_t j);
    void vertexSwap(std::size_t i, std::size_t j);

    void blockInsert(Run::WorkStopIterator i, Run::WorkStopIterator j, Run::WorkStopIterator k);
    void blockReverse(Run::WorkStopIterator i, Run::WorkStopIterator j);
    void vertexInsert(Run::WorkStopIterator i, Run::WorkStopIterator j);
    void vertexSwap(Run::WorkStopIterator i, Run::WorkStopIterator j);

    void printSolution();
    void printSolution(Run& run);

    MutationType selectRandomMutation();

    SceneEditor scene_editor;
    std::mt19937_64 random_engine;
    index_distr_t index_distribution;
    float_distr_t float_distribution;
    Run& run;
    std::set<MutationType> allowed_mutations;
    std::size_t N;
    bool has_permutation;
};

class SolutionGeneratorClassic : public SolutionGenerator
{
public:

    explicit SolutionGeneratorClassic (Run& run);

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
    explicit InstanceBasedSolutionGenerator (Run& run);

    virtual void neighbour () override;
    void setPopulations(std::vector<ReferenceWrapper<Run>> populations);

protected:
    typedef std::vector<std::size_t> VectorSizeT;
    typedef std::pair<Run::WorkStopIterator,Run::WorkStopIterator> WorkStopIteratorPair;

    void neighbour (const VectorSizeT& anotherRun);
    void neighbour (std::size_t idA, std::size_t idB);
    void neighbour (Run::WorkStopIterator a, Run::WorkStopIterator b);

    void addEdgeWithBlockReverse(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithBlockReverseSimple(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithBlockReverseComplex(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithVertexInsert(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithVertexSwap(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithBlockInsert(Run::WorkStopIterator a, Run::WorkStopIterator b);

    void addEdgeWithBlockInsert(std::size_t a, std::size_t b);
    
    bool checkEdge(std::size_t idA, std::size_t idB) const;
    
    MutationType selectMutation(Run::WorkStopIterator a, Run::WorkStopIterator b);

    std::vector<VectorSizeT> optimized_populations;
    std::unordered_map<std::size_t, Run::WorkStopIterator> ids;
};

}
