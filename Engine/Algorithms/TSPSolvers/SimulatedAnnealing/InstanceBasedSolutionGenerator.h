#pragma once

#include "SolutionGenerator.h"
#include <unordered_map>

namespace Scheduler
{

class InstanceBasedSolutionGenerator : public SolutionGenerator
{
public:
    typedef std::vector<std::size_t> VectorSizeT;
    typedef std::vector<VectorSizeT> PopulationsT;

    explicit InstanceBasedSolutionGenerator(Run& run);

    virtual void neighbour() override;
    void setPopulations(const PopulationsT & populations, std::size_t self_index);

protected:
    typedef std::pair<Run::WorkStopIterator,Run::WorkStopIterator> WorkStopIteratorPair;

    void neighbour(const VectorSizeT& another_run);
    void neighbour(std::size_t id1, std::size_t id2);
    void neighbour(Run::WorkStopIterator a, Run::WorkStopIterator b);

    void addEdgeWithBlockReverse(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithBlockReverseDirect(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithBlockReverseCircular(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithVertexInsert(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithVertexSwap(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithBlockInsert(Run::WorkStopIterator a, Run::WorkStopIterator b);

    bool checkEdge(std::size_t id1, std::size_t id2) const;

    MutationType selectMutation(Run::WorkStopIterator a, Run::WorkStopIterator b);

    PopulationsT const * populations_ptr;
    std::unordered_map<std::size_t, Run::WorkStopIterator> ids;
    std::size_t self_index_in_populations;

    Logger& logger;
};

}
