#pragma once

#include <random>
#include <set>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

namespace Scheduler
{

class Run;
class SceneEditor;

class SolutionGenerator
{
public:
    enum class MutationType : unsigned
    {
        BlockInsert = 1,
        BlockReverse = 2,
        VertexInsert = 4,
        VertexSwap = 8
    };

    SolutionGenerator (Run* run);

    void enableMutation (MutationType mutation);
    void disableMutation (MutationType mutation);
    void randomMutation ();
    void shuffle ();
    void discard ();
    void store ();
    size_t checkpoint ();
    void rollbackTo (size_t checkpoint);

private:
    typedef typename std::uniform_int_distribution<size_t> index_distr_t;
    typedef typename std::uniform_real_distribution<float> float_distr_t;
    typedef typename index_distr_t::param_type index_param_t;
    typedef typename float_distr_t::param_type float_param_t;

    void blockInsert();
    void blockReverse();
    void vertexInsert();
    void vertexSwap();

    SceneEditor scene_editor;
    std::mt19937_64 random_engine;
    index_distr_t index_distribution;
    float_distr_t float_distribution;
    ImmutableVector<Run*>::const_iterator run_iter;
    std::set<MutationType> allowed_mutations;
    size_t N;
};
}
