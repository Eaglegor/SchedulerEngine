#include "SolutionGeneratorClassic.h"

namespace Scheduler
{

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

}
