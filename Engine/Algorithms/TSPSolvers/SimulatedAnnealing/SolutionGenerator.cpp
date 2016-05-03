#include "SolutionGenerator.h"
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStop.h>
#include <Engine/SceneEditor/Actions/ScrambleWorkStopsSubsequence.h>
#include <Engine/SceneEditor/Actions/RotateWorkStopsSubsequence.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>

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


void SolutionGenerator::randomMutation()
{
    const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
    const float s = 1.f / allowed_mutations.size();
    float v = s;
    for (auto m : allowed_mutations) {
        if (random_value < v) {
            switch (m) {
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

            break;
        }
        v += s;
    }
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

void SolutionGenerator::blockInsert()
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
    scene_editor.performAction<RotateWorkStopsSubsequence>(run_iter,
                                                           (*run_iter)->getWorkStops().begin() + ri,
                                                           (*run_iter)->getWorkStops().begin() + rj,
                                                           (*run_iter)->getWorkStops().begin() + rk);
}

void SolutionGenerator::blockReverse()
{
    size_t i = index_distribution(random_engine, index_param_t(0, N));
    const size_t L = (i == 0 || i == N) ? 2 : 3;
    size_t j = index_distribution(random_engine, index_param_t(0, N - L));
    if ((j + 1) >= i) {
        j += L;
    }
    const size_t ri = std::min(i, j);
    const size_t rj = std::max(i, j);

    scene_editor.performAction<ReverseWorkStopsSubsequence>(run_iter,
                                                            (*run_iter)->getWorkStops().begin() + ri,
                                                            (*run_iter)->getWorkStops().begin() + rj);
}

void SolutionGenerator::vertexInsert()
{
    size_t i = index_distribution(random_engine, index_param_t(0, N - 1));
    size_t j = index_distribution(random_engine, index_param_t(0, N - 2));
    if (j >= i) {
        ++j;
    }

    scene_editor.performAction<MoveRunWorkStop>(run_iter,
                                                (*run_iter)->getWorkStops().begin() + i,
                                                (*run_iter)->getWorkStops().begin() + j);
}

void SolutionGenerator::vertexSwap()
{
    size_t i = index_distribution(random_engine, index_param_t(0, N - 1));
    size_t j = index_distribution(random_engine, index_param_t(0, N - 2));
    if (j >= i) {
        ++j;
    }

    scene_editor.performAction<SwapRunWorkStops>(run_iter,
                                                 (*run_iter)->getWorkStops().begin() + i,
                                                 (*run_iter)->getWorkStops().begin() + j);
}
}
