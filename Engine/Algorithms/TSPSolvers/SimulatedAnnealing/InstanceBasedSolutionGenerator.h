#pragma once

#include "SolutionGenerator.h"
#include <unordered_map>
#include <limits>

namespace Scheduler
{

class InstanceBasedSolutionGenerator : public SolutionGenerator
{
public:
    explicit InstanceBasedSolutionGenerator(Run& run);

    virtual void neighbour() override;
    void addInstance(const InstanceBasedSolutionGenerator& instance);
    void store();

protected:
    typedef std::uint32_t StopIdT;
    typedef std::pair<StopIdT,StopIdT> EdgeT;
    typedef std::vector<EdgeT> VectorSizeT;
    typedef std::vector<const VectorSizeT *> PopulationsT;
    typedef std::pair<Run::WorkStopIterator,Run::WorkStopIterator> WorkStopIteratorPair;

    static constexpr StopIdT start_stop_id = std::numeric_limits<StopIdT>::max();

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

    void updateEdges();

    MutationType selectMutation(Run::WorkStopIterator a, Run::WorkStopIterator b);

    PopulationsT populations;
    VectorSizeT edges;
    std::unordered_map<StopIdT, Run::WorkStopIterator> ids;

    Logger& logger;
};

}
