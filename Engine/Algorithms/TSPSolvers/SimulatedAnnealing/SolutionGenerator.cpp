#include "SolutionGenerator.h"
#include <iostream>
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
    MutationType mutationType = *allowed_mutations.begin();
    for (auto m : allowed_mutations) {
        if (random_value < v) {
            mutationType = m;
            break;
        }
        v += s;
    }
    return mutationType;
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
    SolutionGenerator(run)
{
    for (Run::WorkStopIterator it = run.getWorkStops().begin();
         it != run.getWorkStops().end();
         ++it) {
        ids[it->getOperation().getId()] = it;
    }
}

void InstanceBasedSolutionGenerator::neighbour()
{
    has_permutation = false;
    std::size_t from = index_distribution(random_engine, index_param_t(0, optimized_populations_ref->size() - 2));
    if (from >= self_index_in_populations) {
        ++from;
    }
    neighbour(optimized_populations_ref->at(from));
}

void InstanceBasedSolutionGenerator::neighbour (const InstanceBasedSolutionGenerator::VectorSizeT& anotherRun)
{
    const std::size_t source_edge = index_distribution(random_engine, index_param_t(0, N));
    const std::size_t operation_a_id = source_edge == 0 ? std::numeric_limits<std::size_t>::max() : anotherRun.at(source_edge - 1);
    const std::size_t operation_b_id = source_edge == N ? std::numeric_limits<std::size_t>::max() : anotherRun.at(source_edge);
    neighbour(operation_a_id, operation_b_id);
    assert(checkEdge(operation_a_id, operation_b_id));
}

void InstanceBasedSolutionGenerator::neighbour (std::size_t idA, std::size_t idB)
{
    if (idA == std::numeric_limits<std::size_t>::max()) {
        const auto operation_b_it = ids.at(idB);
        neighbour(run.getWorkStops().end(), operation_b_it);
    } else if (idB == std::numeric_limits<std::size_t>::max()) {
        const auto operation_a_it = ids.at(idA);
        neighbour(operation_a_it, run.getWorkStops().end());
    } else {
        const auto iter_a = ids.at(idA);
        const auto iter_b = ids.at(idB);
        neighbour(iter_a, iter_b);
    }
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
    addEdgeWithBlockInsert(std::distance(run.getWorkStops().begin(), a == run.getWorkStops().end() ? run.getWorkStops().begin() : std::next(a)),
                           std::distance(run.getWorkStops().begin(), b));
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockInsert(std::size_t a, std::size_t b)
{
    if (a < b) {
        const std::size_t U = N - 1 - (b - a);
        std::size_t new_pos = index_distribution(random_engine, index_param_t(0, U));
        if (new_pos >= a) {
            new_pos += b - a + 1;
        }
        blockInsert(a, b, new_pos);
    } else {
        const std::size_t U = (a - b) - 1;
        const std::size_t u = U > 1 ? index_distribution(random_engine, index_param_t(0, U - 1)) : 0;
        const std::size_t new_pos = b + u + 1;
        blockInsert(b, new_pos, a);
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockReverse(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    if (a == run.getWorkStops().end() ||
        std::distance(run.getWorkStops().begin(), a) < std::distance(run.getWorkStops().begin(), b)) {
        addEdgeWithBlockReverseSimple(a, b);
    } else {
        addEdgeWithBlockReverseComplex(a, b);
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockReverseSimple(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    if (a == std::prev(run.getWorkStops().end()) || b == run.getWorkStops().end()) {
        blockReverse(a, b);
    } else if (b == run.getWorkStops().begin() || a == run.getWorkStops().end()) {
        blockReverse(a == run.getWorkStops().end() ? run.getWorkStops().begin() : std::next(a), std::next(b));
    } else {
        const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
        if (random_value < 0.5f) {
            blockReverse(std::next(a), std::next(b));
        } else {
            blockReverse(a, b);
        }
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockReverseComplex(Run::WorkStopIterator a, Run::WorkStopIterator b)
{
    addEdgeWithVertexSwap(a, b);
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
        vertexSwap(a == run.getWorkStops().end() ? run.getWorkStops().begin() : std::next(a), b);
    } else {
        const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
        if (random_value < 0.5f) {
            vertexSwap(std::next(a), b);
        } else {
            vertexSwap(a, std::prev(b));
        }
    }
}

void InstanceBasedSolutionGenerator::setPopulations(const PopulationsT & populations, std::size_t selfIndexInPopulations)
{
    this->optimized_populations_ref = &populations;
    this->self_index_in_populations = selfIndexInPopulations;
}

bool InstanceBasedSolutionGenerator::checkEdge(std::size_t idA, std::size_t idB) const
{
    bool result = false;
    if (idA == std::numeric_limits<std::size_t>::max()) {
        result = run.getWorkStops().front().getOperation().getId() == idB;
        if (!result) {
            std::cout << "fail at first" << std::endl;
        }
    } else if (idB == std::numeric_limits<std::size_t>::max()) {
        result = run.getWorkStops().back().getOperation().getId() == idA;
        if (!result) {
            std::cout << "fail at last" << std::endl;
        }
    } else {
        auto it = std::find_if(run.getWorkStops().begin(),
                               run.getWorkStops().end(),
                               [&] (const WorkStop & workStop) {
                                   return workStop.getOperation().getId() == idA;
                               });
        result = (it != run.getWorkStops().end() &&
                  it != std::prev(run.getWorkStops().end()) &&
                  std::next(it)->getOperation().getId() == idB);
        if (!result) {
            std::cout << "fail at mid" << std::endl;
        }
    }
    return result;
}

}
