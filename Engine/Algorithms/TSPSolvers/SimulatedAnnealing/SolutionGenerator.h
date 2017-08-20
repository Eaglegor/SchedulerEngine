#pragma once

#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/Run.h>
#include <random>
#include <set>

namespace Scheduler
{

	class SolutionGenerator
	{
	public:
		enum class MutationType : unsigned
		{
			BlockInsert  = 1,
			BlockReverse = 2,
			VertexInsert = 4,
			VertexSwap   = 8,
			None         = 16
		};

		explicit SolutionGenerator(Run& run);

		virtual void neighbour( ) = 0;

		void enableMutation(MutationType mutation);
		void disableMutation(MutationType mutation);
		void discard( );
		void store( );
		void shuffle( );
		std::size_t checkpoint( );
		void rollbackTo(std::size_t checkpoint);
		bool hasPermutation( ) const;

	protected:
		typedef std::uniform_int_distribution<std::size_t> index_distr_t;
		typedef std::uniform_real_distribution<float> float_distr_t;
		typedef index_distr_t::param_type index_param_t;
		typedef float_distr_t::param_type float_param_t;

		void blockInsert(std::size_t i, std::size_t j, std::size_t k);
		void blockReverse(std::size_t i, std::size_t j);
		void vertexInsert(std::size_t i, std::size_t j);
		void vertexSwap(std::size_t i, std::size_t j);

		void blockInsert(Run::WorkStopIterator i, Run::WorkStopIterator j, Run::WorkStopIterator k);
		void blockReverse(Run::WorkStopIterator i, Run::WorkStopIterator j);
		void vertexInsert(Run::WorkStopIterator i, Run::WorkStopIterator j);
		void vertexSwap(Run::WorkStopIterator i, Run::WorkStopIterator j);

		void printSolution( );

		MutationType selectRandomMutation( );

		Run& run;
		std::set<MutationType> allowed_mutations;
		std::size_t N;
		SceneEditor scene_editor;
		std::mt19937_64 random_engine;
		index_distr_t index_distribution;
		float_distr_t float_distribution;
		bool has_permutation;
	};
}
