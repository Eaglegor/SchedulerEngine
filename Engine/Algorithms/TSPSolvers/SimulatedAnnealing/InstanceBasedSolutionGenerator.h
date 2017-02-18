#pragma once

#include "SolutionGenerator.h"
#include <unordered_map>
#include <limits>

namespace Scheduler
{

class InstanceBasedSolutionGenerator : public SolutionGenerator
{
public:
    typedef std::uint32_t StopIdT;
    typedef std::pair<StopIdT,StopIdT> EdgeT;
    typedef std::vector<EdgeT> VectorSizeT;
    typedef std::vector<VectorSizeT> PopulationsT;

    static constexpr StopIdT start_stop_id = std::numeric_limits<StopIdT>::max();

    explicit InstanceBasedSolutionGenerator(Run& run);

    virtual void neighbour() override;
    void setPopulations(const PopulationsT & populations, std::size_t self_index);

protected:
    typedef std::pair<Run::WorkStopIterator,Run::WorkStopIterator> WorkStopIteratorPair;

    void neighbour(const VectorSizeT& another_run);
    void neighbour(StopIdT id1, StopIdT id2);
    void neighbour(Run::WorkStopIterator a, Run::WorkStopIterator b);

    void addEdgeWithBlockReverse(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithBlockReverseDirect(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithBlockReverseCircular(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithVertexInsert(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithVertexSwap(Run::WorkStopIterator a, Run::WorkStopIterator b);
    void addEdgeWithBlockInsert(Run::WorkStopIterator a, Run::WorkStopIterator b);

    bool checkEdge(StopIdT id1, StopIdT id2) const;

    MutationType selectMutation(Run::WorkStopIterator a, Run::WorkStopIterator b);

    PopulationsT const * populations_ptr;
    std::unordered_map<StopIdT, Run::WorkStopIterator> ids;
    std::size_t self_index_in_populations;

    Logger& logger;
};

}
