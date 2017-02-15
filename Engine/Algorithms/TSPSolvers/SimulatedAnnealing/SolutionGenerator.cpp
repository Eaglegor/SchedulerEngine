#include "SolutionGenerator.h"
#include <limits>
#include <Engine/SceneEditor/Actions/SwapWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseWorkStops.h>
#include <Engine/SceneEditor/Actions/MoveWorkStop.h>
#include <Engine/SceneEditor/Actions/ScrambleWorkStops.h>
#include <Engine/SceneEditor/Actions/RotateWorkStops.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>

namespace Scheduler
{

SolutionGenerator::SolutionGenerator (Run& run):
run(run)
{
    N = run.getWorkStops().size();
    std::random_device random_device;
    random_engine.seed(random_device());
}

void SolutionGenerator::enableMutation (MutationType mutation)
{
    allowed_mutations.insert(mutation);
}

void SolutionGenerator::disableMutation (MutationType mutation)
{
    allowed_mutations.erase(mutation);
}

void SolutionGenerator::discard ()
{
    scene_editor.rollbackAll();
}

void SolutionGenerator::store ()
{
    scene_editor.commit();
}

void SolutionGenerator::shuffle ()
{
    has_permutation = true;
    scene_editor.performAction<ScrambleWorkStops>(run, run.getWorkStops().begin(), run.getWorkStops().end());
}

size_t SolutionGenerator::checkpoint ()
{
    return scene_editor.checkpoint();
}

void SolutionGenerator::rollbackTo (std::size_t checkpoint)
{
    scene_editor.rollbackToCheckpoint(checkpoint);
}

SolutionGenerator::MutationType SolutionGenerator::selectRandomMutation()
{
    const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
    const float s = 1.f / allowed_mutations.size();
    float v = s;
    MutationType mutation_type = *allowed_mutations.begin();
    for (auto m : allowed_mutations) {
        if (random_value < v) {
            mutation_type = m;
            break;
        }
        v += s;
    }
    return mutation_type;
}

void SolutionGenerator::blockInsert(std::size_t i, std::size_t j, std::size_t k)
{
    auto iter_ri = std::next(run.getWorkStops().begin(), i);
    auto iter_rj = std::next(iter_ri, j - i);
    auto iter_rk = std::next(run.getWorkStops().begin(), k);

    blockInsert(iter_ri, iter_rj, iter_rk);
}

void SolutionGenerator::blockReverse(std::size_t i, std::size_t j)
{
    auto iter_ri = std::next(run.getWorkStops().begin(), i);
    auto iter_rj = std::next(iter_ri, j - i);

    blockReverse(iter_ri, iter_rj);
}

void SolutionGenerator::vertexInsert(std::size_t i, std::size_t j)
{
    Run::WorkStopsList::iterator iter_i;
    Run::WorkStopsList::iterator iter_j;
    if (j > i) {
        iter_i = std::next(run.getWorkStops().begin(), i);
        iter_j = std::next(iter_i, j - i);
    } else {
        iter_j = std::next(run.getWorkStops().begin(), j);
        iter_i = std::next(iter_j, i - j);
    }

    vertexInsert(iter_i, iter_j);
}

void SolutionGenerator::vertexSwap(std::size_t i, std::size_t j)
{
    Run::WorkStopsList::iterator iter_i;
    Run::WorkStopsList::iterator iter_j;
    if (j > i) {
        iter_i = std::next(run.getWorkStops().begin(), i);
        iter_j = std::next(iter_i, j - i);
    } else {
        iter_j = std::next(run.getWorkStops().begin(), j);
        iter_i = std::next(iter_j, i - j);
    }

    vertexSwap(iter_i, iter_j);
}

void SolutionGenerator::blockInsert(Run::WorkStopIterator i, Run::WorkStopIterator j, Run::WorkStopIterator k)
{
    has_permutation = true;
    scene_editor.performAction<RotateWorkStops>(run, i, j, k);
}

void SolutionGenerator::blockReverse(Run::WorkStopIterator i, Run::WorkStopIterator j)
{
    has_permutation = true;
    scene_editor.performAction<ReverseWorkStops>(run, i, j);
}

void SolutionGenerator::vertexInsert(Run::WorkStopIterator i, Run::WorkStopIterator j)
{
    has_permutation = true;
    scene_editor.performAction<MoveWorkStop>(run, i, j);
}

void SolutionGenerator::vertexSwap(Run::WorkStopIterator i, Run::WorkStopIterator j)
{
    has_permutation = true;
    scene_editor.performAction<SwapWorkStops>(run, i, j);
}

void SolutionGenerator::printSolution()
{
    printSolution(run);
}

void SolutionGenerator::printSolution(Run& run)
{
    std::cout << std::endl << "solution order:" << std::endl;
    for (auto it = run.getWorkStops().begin(); it != run.getWorkStops().end(); ++it) {
        std::cout << it->getOperation().getId() << " ";
    }
    std::cout << std::endl;
}

bool SolutionGenerator::hasPermutation() const
{
    return has_permutation;
}






SolutionGeneratorClassic::SolutionGeneratorClassic(Run &run) : SolutionGenerator(run)
{}

void SolutionGeneratorClassic::neighbour()
{
    has_permutation = false;
    randomMutation();
}

void SolutionGeneratorClassic::randomMutation()
{
    switch (selectRandomMutation()) {
    case MutationType::BlockInsert:
        blockInsert();
        break;
    case MutationType::BlockReverse:
        blockReverse();
        break;
    case MutationType::VertexInsert:
        vertexInsert();
        break;
    case MutationType::VertexSwap:
        vertexSwap();
        break;
    default:
        break;
    }
}

void SolutionGeneratorClassic::blockInsert()
{
    std::size_t i = index_distribution(random_engine, index_param_t(0, N));
    std::size_t j = index_distribution(random_engine, index_param_t(0, N - 1));
    std::size_t k = index_distribution(random_engine, index_param_t(0, N - 2));
    if (j >= i) {
        ++j;
    }
    if (k >= i) {
        ++k;
    }
    if (k >= j) {
        ++k;
        if (k == i) {
            ++k;
        }
    }

    const std::size_t ri = std::min({i, j, k});
    const std::size_t rk = std::max({i, j, k});
    const std::size_t rj = i + j + k - ri - rk;

    SolutionGenerator::blockInsert(ri, rj, rk);
}

void SolutionGeneratorClassic::blockReverse()
{
    std::size_t i = index_distribution(random_engine, index_param_t(0, N));
    const std::size_t L = (i == 0 || i == N) ? 2 : 3;
    std::size_t j = index_distribution(random_engine, index_param_t(0, N - L));
    if ((j + 1) >= i) {
        j += L;
    }
    const std::size_t ri = std::min(i, j);
    const std::size_t rj = std::max(i, j);

    SolutionGenerator::blockReverse(ri, rj);
}

void SolutionGeneratorClassic::vertexInsert()
{
    std::size_t i = index_distribution(random_engine, index_param_t(0, N - 1));
    std::size_t j = index_distribution(random_engine, index_param_t(0, N - 2));
    if (j >= i) {
        ++j;
    }

    SolutionGenerator::vertexInsert(i, j);
}

void SolutionGeneratorClassic::vertexSwap()
{
    std::size_t i = index_distribution(random_engine, index_param_t(0, N - 1));
    std::size_t j = index_distribution(random_engine, index_param_t(0, N - 2));
    if (j >= i) {
        ++j;
    }

    SolutionGenerator::vertexSwap(i, j);
}









InstanceBasedSolutionGenerator::InstanceBasedSolutionGenerator(Run &run) :
    SolutionGenerator(run),
    logger(LoggingManager::getLogger("InstanceBasedSolutionGenerator"))
{
    for (Run::WorkStopIterator it = run.getWorkStops().begin(); it != run.getWorkStops().end(); ++it) {
        ids[it->getOperation().getId()] = it;
    }
}

void InstanceBasedSolutionGenerator::neighbour()
{
    has_permutation = false;
    std::size_t from = index_distribution(random_engine, index_param_t(0, populations_ptr->size() - 2));
    if (from >= self_index_in_populations) {
        ++from;
    }
    neighbour(populations_ptr->at(from));
}

void InstanceBasedSolutionGenerator::neighbour (const InstanceBasedSolutionGenerator::VectorSizeT& another_run)
{
    const std::size_t source_edge = index_distribution(random_engine, index_param_t(0, N));
    const std::size_t operation_a_id = (source_edge == 0 ? std::numeric_limits<std::size_t>::max() : another_run.at(source_edge - 1));
    const std::size_t operation_b_id = (source_edge == N ? std::numeric_limits<std::size_t>::max() : another_run.at(source_edge));
    neighbour(operation_a_id, operation_b_id);
    assert(checkEdge(operation_a_id, operation_b_id));
}

void InstanceBasedSolutionGenerator::neighbour (std::size_t a_id, std::size_t b_id)
{
    const auto iter_a = (a_id != std::numeric_limits<std::size_t>::max() ? ids.at(a_id) : run.getWorkStops().end());
    const auto iter_b = (b_id != std::numeric_limits<std::size_t>::max() ? ids.at(b_id) : run.getWorkStops().end());
    neighbour(iter_a, iter_b);
}

void InstanceBasedSolutionGenerator::neighbour (Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    switch (selectMutation(a, b)) {
        case MutationType::BlockReverse:
            addEdgeWithBlockReverse(a, b);
            break;
        case MutationType::VertexInsert:
            addEdgeWithVertexInsert(a, b);
            break;
        case MutationType::VertexSwap:
            addEdgeWithVertexSwap(a, b);
            break;
        case MutationType::BlockInsert:
            addEdgeWithBlockInsert(a, b);
            break;
        default:
            break;
    }
}

SolutionGenerator::MutationType InstanceBasedSolutionGenerator::selectMutation(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    assert(a != run.getWorkStops().end() || b != run.getWorkStops().end());

    if (b == run.getWorkStops().begin() && a == std::prev(run.getWorkStops().end())) {
        return MutationType::VertexInsert;
    }

    if ((a != run.getWorkStops().end() && std::next(a) == b) ||
        (a == run.getWorkStops().end() && b == run.getWorkStops().begin())) {
        return MutationType::None;
    }

    return selectRandomMutation();
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockInsert(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    a = (a == run.getWorkStops().end() ? run.getWorkStops().begin() : std::next(a));
    const std::size_t a_idx = std::distance(run.getWorkStops().begin(), a);
    const std::size_t b_idx = std::distance(run.getWorkStops().begin(), b);

    if (a_idx < b_idx) {
        const std::size_t U = N - 1 - (b_idx - a_idx);
        std::size_t c_idx = index_distribution(random_engine, index_param_t(0, U));
        if (c_idx >= a_idx) {
            c_idx += b_idx - a_idx + 1;
        }
        auto c = c_idx < a_idx ? std::prev(a, a_idx - c_idx) : std::next(b, c_idx - b_idx);

        blockInsert(a, b, c);
    } else {
        const std::size_t U = (a_idx - b_idx) - 1;
        const std::size_t u = U > 1 ? index_distribution(random_engine, index_param_t(0, U - 1)) : 0;
        const std::size_t c_idx = b_idx + u + 1;
        auto c = std::next(b, c_idx - b_idx);

        blockInsert(b, c, a);
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockReverse(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    if (a == run.getWorkStops().end() ||
        std::distance(run.getWorkStops().begin(), a) < std::distance(run.getWorkStops().begin(), b)) {
        addEdgeWithBlockReverseDirect(a, b);
    } else {
        addEdgeWithBlockReverseCircular(a, b);
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockReverseDirect(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    if (a == std::prev(run.getWorkStops().end()) || b == run.getWorkStops().end()) {
        blockReverse(a, b);
    } else if (b == run.getWorkStops().begin() || a == run.getWorkStops().end()) {
        auto next_a = (a == run.getWorkStops().end() ? run.getWorkStops().begin() : std::next(a));
        blockReverse(next_a, std::next(b));
    } else {
        const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
        if (random_value < 0.5f) {
            blockReverse(std::next(a), std::next(b));
        } else {
            blockReverse(a, b);
        }
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockReverseCircular(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    //real circular reverse not good by quality / performance
    addEdgeWithBlockInsert(a, b);
}

void InstanceBasedSolutionGenerator::addEdgeWithVertexInsert(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    if (b == run.getWorkStops().end()) {
        vertexInsert(a, b);
    } else if (a == run.getWorkStops().end()) {
        vertexInsert(b, run.getWorkStops().begin());
    } else {
        const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
        if (random_value < 0.5f) {
            vertexInsert(b, std::next(a));
        } else {
            vertexInsert(a, b);
        }
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithVertexSwap(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    if (a == std::prev(run.getWorkStops().end()) || b == run.getWorkStops().end()) {
        vertexSwap(a, std::prev(b));
    } else if (b == run.getWorkStops().begin() || a == run.getWorkStops().end()) {
        auto next_a = (a == run.getWorkStops().end() ? run.getWorkStops().begin() : std::next(a));
        vertexSwap(next_a, b);
    } else {
        const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
        if (random_value < 0.5f) {
            vertexSwap(std::next(a), b);
        } else {
            vertexSwap(a, std::prev(b));
        }
    }
}

void InstanceBasedSolutionGenerator::setPopulations(const PopulationsT & populations, std::size_t self_index)
{
    this->populations_ptr = &populations;
    this->self_index_in_populations = self_index;
}

bool InstanceBasedSolutionGenerator::checkEdge(std::size_t a_id, std::size_t b_id) const
{
    bool result = false;
    if (a_id == std::numeric_limits<std::size_t>::max()) {
        result = run.getWorkStops().front().getOperation().getId() == b_id;
        if (!result) {
            LOG_ERROR(logger, "check edge first - {} failed", b_id);
        }
    } else if (b_id == std::numeric_limits<std::size_t>::max()) {
        result = run.getWorkStops().back().getOperation().getId() == a_id;
        if (!result) {
            LOG_ERROR(logger, "check edge {} - last failed", a_id);
        }
    } else {
        auto it = std::find_if(run.getWorkStops().begin(),
                               run.getWorkStops().end(),
                               [&] (const WorkStop & workStop) {
                                   return workStop.getOperation().getId() == a_id;
                               });
        result = (it != run.getWorkStops().end() &&
                  it != std::prev(run.getWorkStops().end()) &&
                  std::next(it)->getOperation().getId() == b_id);
        if (!result) {
            LOG_ERROR(logger, "check edge {} - {} failed", a_id, b_id);
        }
    }
    return result;
}

}
