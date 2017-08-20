#include "SolutionGenerator.h"
#include <Engine/SceneEditor/Actions/SwapWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseWorkStops.h>
#include <Engine/SceneEditor/Actions/MoveWorkStop.h>
#include <Engine/SceneEditor/Actions/ScrambleWorkStops.h>
#include <Engine/SceneEditor/Actions/RotateWorkStops.h>
#include <Engine/SceneManager/WorkOperation.h>
#include <Engine/SceneManager/DepotOperation.h>

namespace Scheduler
{

SolutionGenerator::SolutionGenerator(Run& run):
run(run)
{
    N = run.getWorkStops().size();
    std::random_device random_device;
    random_engine.seed(random_device());
}

void SolutionGenerator::enableMutation(MutationType mutation)
{
    allowed_mutations.insert(mutation);
}

void SolutionGenerator::disableMutation(MutationType mutation)
{
    allowed_mutations.erase(mutation);
}

void SolutionGenerator::discard()
{
    scene_editor.rollbackAll();
}

void SolutionGenerator::store()
{
    scene_editor.commit();
}

void SolutionGenerator::shuffle()
{
    has_permutation = true;
    scene_editor.performAction<ScrambleWorkStops>(run, run.getWorkStops().begin(), run.getWorkStops().end());
}

size_t SolutionGenerator::checkpoint()
{
    return scene_editor.checkpoint();
}

void SolutionGenerator::rollbackTo(std::size_t checkpoint)
{
    scene_editor.rollbackToCheckpoint(checkpoint);
}

SolutionGenerator::MutationType SolutionGenerator::selectRandomMutation()
{
    if (run.getWorkStops().size() <= 2) {
        return MutationType::VertexSwap;
    }

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

}
