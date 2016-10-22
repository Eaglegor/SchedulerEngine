#include "SolutionGenerator.h"
#include <iostream>
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

void SolutionGenerator::shuffle ()
{
    scene_editor.performAction<ScrambleWorkStops>(run, run.getWorkStops().begin(), run.getWorkStops().end());
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

    scene_editor.performAction<RotateWorkStops>(run,
                                                iter_ri,
                                                iter_rj,
                                                iter_rk);
}

void SolutionGenerator::blockReverse(std::size_t i, std::size_t j)
{
    auto iter_ri = std::next(run.getWorkStops().begin(), i);
    auto iter_rj = std::next(iter_ri, j - i);

    scene_editor.performAction<ReverseWorkStops>(run,
                                                 iter_ri,
                                                 iter_rj);
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

    scene_editor.performAction<MoveWorkStop>(run,
                                                iter_i,
                                                iter_j);
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

    scene_editor.performAction<SwapWorkStops>(run,
                                              iter_i,
                                              iter_j);
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







SolutionGeneratorClassic::SolutionGeneratorClassic(Run &run) : SolutionGenerator(run)
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
{}

void InstanceBasedSolutionGenerator::neighbour()
{
    const std::size_t from = index_distribution(random_engine, index_param_t(0, populations.size() - 1));
    neighbour(populations.at(from));
}

void InstanceBasedSolutionGenerator::neighbour (Run& anotherRun)
{
    const std::size_t source_edge = index_distribution(random_engine, index_param_t(0, N));

    if (source_edge == 0) {
        auto source_iter_b = anotherRun.getWorkStops().begin();
        const std::size_t operation_b_id = source_iter_b->getOperation().getId();

        bool operation_b_found = false;
        std::size_t operation_b_idx = 0;
        std::size_t operation_idx = 0;
        for (auto iter = run.getWorkStops().begin();
             iter != run.getWorkStops().end() && !operation_b_found;
             ++iter, ++operation_idx) {
            const std::size_t operation_id = iter->getOperation().getId();
            if (operation_id == operation_b_id) {
                operation_b_found = true;
                operation_b_idx = operation_idx;
            }
        }
        if (operation_b_found) {
            neighbour(0, operation_b_idx, false);
        } else {
            std::cout << "oops!";
        }
    } else if (source_edge == N) {
        auto source_iter_a = anotherRun.getWorkStops().begin();
        const std::size_t operation_a_id = source_iter_a->getOperation().getId();

        bool operation_a_found = false;
        std::size_t operation_a_idx = 0;
        std::size_t operation_idx = 0;
        for (auto iter = run.getWorkStops().begin();
             iter != run.getWorkStops().end() && !operation_a_found;
             ++iter, ++operation_idx) {
            const std::size_t operation_id = iter->getOperation().getId();
            if (operation_id == operation_a_id) {
                operation_a_found = true;
                operation_a_idx = operation_idx;
            }
        }
        if (operation_a_found) {
            neighbour(operation_a_idx, N - 1, true);
        } else {
            std::cout << "oops!";
        }
    } else {
        auto source_iter_a = std::next(anotherRun.getWorkStops().begin(), source_edge - 1);
        auto source_iter_b = std::next(source_iter_a, 1);
        const std::size_t operation_a_id = source_iter_a->getOperation().getId();
        const std::size_t operation_b_id = source_iter_b->getOperation().getId();

        bool operation_a_found = false;
        bool operation_b_found = false;
        std::size_t operation_a_idx = 0;
        std::size_t operation_b_idx = 0;
        std::size_t operation_idx = 0;
        for (auto iter = run.getWorkStops().begin();
             iter != run.getWorkStops().end() && (!operation_a_found || !operation_b_found);
             ++iter, ++operation_idx) {
            const std::size_t operation_id = iter->getOperation().getId();
            if (operation_id == operation_a_id) {
                operation_a_found = true;
                operation_a_idx = operation_idx;
            } else if (operation_id == operation_b_id) {
                operation_b_found = true;
                operation_b_idx = operation_idx;
            }
        }
        if (operation_a_found && operation_b_found) {
            if (operation_a_idx < (N - 1) &&
                operation_b_idx > 0) {
                const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
                if (random_value < 0.5f) {
                    neighbour(operation_a_idx + 1, operation_b_idx, false);
                } else {
                    neighbour(operation_a_idx, operation_b_idx - 1, true);
                }
            } else if (operation_a_idx < (N - 1)) {
                neighbour(operation_a_idx + 1, operation_b_idx, false);
            } else if (operation_b_idx > 0) {
                neighbour(operation_a_idx, operation_b_idx - 1, true);
            }
        } else {
            std::cout << "oops!";
        }
    }
}

void InstanceBasedSolutionGenerator::neighbour(std::size_t a, std::size_t b, bool alternative)
{
    if (a == b) {
        return;
    }
    if ((a + 1) == b) {
        addEdgeWithVertexSwap(a, b);
        return;
    }
    switch (selectRandomMutation()) {
    case MutationType::BlockReverse:
        addEdgeWithBlockReverse(a, b);
        break;
    case MutationType::VertexInsert:
        if (alternative) {
            addEdgeWithVertexInsert(b + 1, a);
        } else {
            addEdgeWithVertexInsert(a, b);
        }
        break;
    case MutationType::VertexSwap:
        addEdgeWithVertexSwap(a, b);
        break;
    case MutationType::BlockInsert:
        if (alternative) {
            addEdgeWithBlockInsert(a + 1, b + 1);
        } else {
            addEdgeWithBlockInsert(a, b);
        }
        break;
    default:
        break;
    }
}

void InstanceBasedSolutionGenerator::addEdgeWithBlockReverse(std::size_t a, std::size_t b)
{
    if (a < b) {
        blockReverse(a, b + 1);
    } else {
        vertexSwap(a, b);
    }
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

void InstanceBasedSolutionGenerator::addEdgeWithVertexInsert(std::size_t a, std::size_t b)
{
    vertexInsert(b, a);
}

void InstanceBasedSolutionGenerator::addEdgeWithVertexSwap(std::size_t a, std::size_t b)
{
    vertexSwap(a, b);
}

void InstanceBasedSolutionGenerator::setPopulations(std::vector<ReferenceWrapper<Run>> populations)
{
    this->populations.clear();
    for (Run& runRef : populations) {
        if (&run != &runRef) {
            this->populations.emplace_back(runRef);
        }
    }
}

}
