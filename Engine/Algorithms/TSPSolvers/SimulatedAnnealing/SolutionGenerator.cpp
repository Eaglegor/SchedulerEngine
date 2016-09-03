#include "SolutionGenerator.h"
#include <iostream>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStop.h>
#include <Engine/SceneEditor/Actions/ScrambleWorkStopsSubsequence.h>
#include <Engine/SceneEditor/Actions/RotateWorkStopsSubsequence.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>

namespace Scheduler
{

SolutionGenerator::SolutionGenerator (Run* run)
{
    run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
    N = run->getWorkStops().size();
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

void SolutionGenerator::shuffle ()
{
    scene_editor.performAction<ScrambleWorkStopsSubsequence>(run_iter,
                                                             (*run_iter)->getWorkStops().begin(),
                                                             (*run_iter)->getWorkStops().end());
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

void SolutionGenerator::rollbackTo (size_t checkpoint)
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

void SolutionGenerator::blockInsert(size_t i, size_t j, size_t k)
{
    auto iter_ri = std::next((*run_iter)->getWorkStops().begin(), i);
    auto iter_rj = std::next(iter_ri, j - i);
    auto iter_rk = std::next((*run_iter)->getWorkStops().begin(), k);

    scene_editor.performAction<RotateWorkStopsSubsequence>(run_iter,
                                                           iter_ri,
                                                           iter_rj,
                                                           iter_rk);
}

void SolutionGenerator::blockReverse(size_t i, size_t j)
{
    auto iter_ri = std::next((*run_iter)->getWorkStops().begin(), i);
    auto iter_rj = std::next(iter_ri, j - i);

    scene_editor.performAction<ReverseWorkStopsSubsequence>(run_iter,
                                                            iter_ri,
                                                            iter_rj);
}

void SolutionGenerator::vertexInsert(size_t i, size_t j)
{
    Run::WorkStopsList::iterator iter_i;
    Run::WorkStopsList::iterator iter_j;
    if (j > i) {
        iter_i = std::next((*run_iter)->getWorkStops().begin(), i);
        iter_j = std::next(iter_i, j - i);
    } else {
        iter_j = std::next((*run_iter)->getWorkStops().begin(), j);
        iter_i = std::next(iter_j, i - j);
    }

    scene_editor.performAction<MoveRunWorkStop>(run_iter,
                                                iter_i,
                                                iter_j);
}

void SolutionGenerator::vertexSwap(size_t i, size_t j)
{
    Run::WorkStopsList::iterator iter_i;
    Run::WorkStopsList::iterator iter_j;
    if (j > i) {
        iter_i = std::next((*run_iter)->getWorkStops().begin(), i);
        iter_j = std::next(iter_i, j - i);
    } else {
        iter_j = std::next((*run_iter)->getWorkStops().begin(), j);
        iter_i = std::next(iter_j, i - j);
    }

    scene_editor.performAction<SwapRunWorkStops>(run_iter,
                                                 iter_i,
                                                 iter_j);
}

void SolutionGenerator::printSolution()
{
    printSolution(*run_iter);
}

void SolutionGenerator::printSolution(Run* run)
{
    std::cout << std::endl << "solution order:" << std::endl;
    for (auto it = run->getWorkStops().begin(); it != run->getWorkStops().end(); ++it) {
        std::cout << (*it)->getOperation()->getId() << " ";
    }
    std::cout << std::endl;
}







SolutionGeneratorClassic::SolutionGeneratorClassic(Run *run) : SolutionGenerator(run)
{}

void SolutionGeneratorClassic::neighbour()
{
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
    size_t i = index_distribution(random_engine, index_param_t(0, N));
    size_t j = index_distribution(random_engine, index_param_t(0, N - 1));
    size_t k = index_distribution(random_engine, index_param_t(0, N - 2));
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

    const size_t ri = std::min({i, j, k});
    const size_t rk = std::max({i, j, k});
    const size_t rj = i + j + k - ri - rk;

    SolutionGenerator::blockInsert(ri, rj, rk);
}

void SolutionGeneratorClassic::blockReverse()
{
    size_t i = index_distribution(random_engine, index_param_t(0, N));
    const size_t L = (i == 0 || i == N) ? 2 : 3;
    size_t j = index_distribution(random_engine, index_param_t(0, N - L));
    if ((j + 1) >= i) {
        j += L;
    }
    const size_t ri = std::min(i, j);
    const size_t rj = std::max(i, j);

    SolutionGenerator::blockReverse(ri, rj);
}

void SolutionGeneratorClassic::vertexInsert()
{
    size_t i = index_distribution(random_engine, index_param_t(0, N - 1));
    size_t j = index_distribution(random_engine, index_param_t(0, N - 2));
    if (j >= i) {
        ++j;
    }

    SolutionGenerator::vertexInsert(i, j);
}

void SolutionGeneratorClassic::vertexSwap()
{
    size_t i = index_distribution(random_engine, index_param_t(0, N - 1));
    size_t j = index_distribution(random_engine, index_param_t(0, N - 2));
    if (j >= i) {
        ++j;
    }

    SolutionGenerator::vertexSwap(i, j);
}









InstanceBasedSolutionGenerator::InstanceBasedSolutionGenerator(Run *run, ScheduleCostFunction* scheduleCostFunction) : SolutionGenerator(run), schedule_cost_function(scheduleCostFunction)
{}

void InstanceBasedSolutionGenerator::neighbour()
{
    const size_t from = index_distribution(random_engine, index_param_t(0, populations.size() - 1));
    neighbour(populations.at(from));
}

void InstanceBasedSolutionGenerator::neighbour (Run* anotherRun)
{
    const size_t source_edge = index_distribution(random_engine, index_param_t(0, N));

    if (source_edge == 0) {
        auto source_iter_b = anotherRun->getWorkStops().begin();
        const size_t operation_b_id = (*source_iter_b)->getOperation()->getId();

        bool operation_b_found = false;
        size_t operation_b_idx = 0;
        size_t operation_idx = 0;
        for (auto iter = (*run_iter)->getWorkStops().begin();
             iter != (*run_iter)->getWorkStops().end() && !operation_b_found;
             ++iter, ++operation_idx) {
            const size_t operation_id = (*iter)->getOperation()->getId();
            if (operation_id == operation_b_id) {
                operation_b_found = true;
                operation_b_idx = operation_idx;
            }
        }
        if (operation_b_found) {
            neighbour(0, operation_b_idx);
        } else {
            std::cout << "oops!";
        }
    } else if (source_edge == N) {
        auto source_iter_a = anotherRun->getWorkStops().begin();
        const size_t operation_a_id = (*source_iter_a)->getOperation()->getId();

        bool operation_a_found = false;
        size_t operation_a_idx = 0;
        size_t operation_idx = 0;
        for (auto iter = (*run_iter)->getWorkStops().begin();
             iter != (*run_iter)->getWorkStops().end() && !operation_a_found;
             ++iter, ++operation_idx) {
            const size_t operation_id = (*iter)->getOperation()->getId();
            if (operation_id == operation_a_id) {
                operation_a_found = true;
                operation_a_idx = operation_idx;
            }
        }
        if (operation_a_found) {
            neighbour(operation_a_idx, N - 1);
        } else {
            std::cout << "oops!";
        }
    } else {
        auto source_iter_a = std::next(anotherRun->getWorkStops().begin(), source_edge - 1);
        auto source_iter_b = std::next(source_iter_a, 1);
        const size_t operation_a_id = (*source_iter_a)->getOperation()->getId();
        const size_t operation_b_id = (*source_iter_b)->getOperation()->getId();

        bool operation_a_found = false;
        bool operation_b_found = false;
        size_t operation_a_idx = 0;
        size_t operation_b_idx = 0;
        size_t operation_idx = 0;
        for (auto iter = (*run_iter)->getWorkStops().begin();
             iter != (*run_iter)->getWorkStops().end() && (!operation_a_found || !operation_b_found);
             ++iter, ++operation_idx) {
            const size_t operation_id = (*iter)->getOperation()->getId();
            if (operation_id == operation_a_id) {
                operation_a_found = true;
                operation_a_idx = operation_idx;
            } else if (operation_id == operation_b_id) {
                operation_b_found = true;
                operation_b_idx = operation_idx;
            }
        }
        if (operation_a_found && operation_b_found) {
            if ((operation_a_idx + 1) < N) {
                neighbour(operation_a_idx + 1, operation_b_idx);
            }
        } else {
            std::cout << "oops!";
        }
    }
}

void InstanceBasedSolutionGenerator::neighbour(size_t a, size_t b)
{
    if (a == b || ((a + 1) == b)) {
        return;
    }
    switch (selectRandomMutation()) {
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

void InstanceBasedSolutionGenerator::addEdgeWithBlockReverse(size_t a, size_t b)
{
    if (a < b) {
        blockReverse(a, b + 1);
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockInsert(size_t a, size_t b)
{
    if (a < b) {
        const size_t U = N - 1 - (b - a);
        size_t new_pos = index_distribution(random_engine, index_param_t(0, U));
        if (new_pos >= a) {
            new_pos += b - a + 1;
        }
        blockInsert(a, b, new_pos);
    } else {
        const size_t U = (a - b) - 1;
        const size_t u = U > 1 ? index_distribution(random_engine, index_param_t(0, U - 1)) : 0;
        const size_t new_pos = b + u + 1;
        blockInsert(b, new_pos, a);
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithVertexInsert(size_t a, size_t b)
{
    vertexInsert(b, a);
}

void InstanceBasedSolutionGenerator::addEdgeWithVertexSwap(size_t a, size_t b)
{
    vertexSwap(a, b);
}

void InstanceBasedSolutionGenerator::setPopulations(std::vector<Run *> populations)
{
    this->populations.clear();
    for (auto runPtr : populations) {
        if (*run_iter != runPtr) {
            this->populations.push_back(runPtr);
        }
    }
}

SolutionGenerator::MutationType InstanceBasedSolutionGenerator::selectBestMutation(long a, long b)
{
    MutationType result = MutationType::None;
    Cost best_cost;
    addEdgeWithBlockReverse(a, b);
    {
        const Cost cost = schedule_cost_function->calculateCost((*run_iter)->getSchedule());
        discard();
        if (result == MutationType::None || cost < best_cost) {
            best_cost = cost;
            result = MutationType::BlockReverse;
        }
    }
    addEdgeWithVertexInsert(a, b);
    {
        const Cost cost = schedule_cost_function->calculateCost((*run_iter)->getSchedule());
        discard();
        if (result == MutationType::None || cost < best_cost) {
            best_cost = cost;
            result = MutationType::VertexInsert;
        }
    }
    addEdgeWithVertexSwap(a, b);
    {
        const Cost cost = schedule_cost_function->calculateCost((*run_iter)->getSchedule());
        discard();
        if (result == MutationType::None || cost < best_cost) {
            best_cost = cost;
            result = MutationType::VertexSwap;
        }
    }
    return result;
}

}
