#include "InstanceBasedSolutionGenerator.h"
#include <Engine/SceneManager/DepotOperation.h>
#include <Engine/SceneManager/WorkOperation.h>

namespace Scheduler
{

	constexpr InstanceBasedSolutionGenerator::StopIdT InstanceBasedSolutionGenerator::start_stop_id;

	InstanceBasedSolutionGenerator::InstanceBasedSolutionGenerator(Run& run)
	    : SolutionGenerator(run),
	      logger(LoggingManager::getLogger("InstanceBasedSolutionGenerator"))
	{
		for(Run::WorkStopIterator it = run.getWorkStops( ).begin( ); it != run.getWorkStops( ).end( ); ++it)
		{
			ids[it->getOperation( ).getId( )] = it;
		}
	}

	void InstanceBasedSolutionGenerator::neighbour( )
	{
		has_permutation  = false;
		std::size_t from = index_distribution(random_engine, index_param_t(0, populations.size( ) - 1));
		neighbour(*populations.at(from));
	}

	void InstanceBasedSolutionGenerator::neighbour(const InstanceBasedSolutionGenerator::VectorSizeT& another_run)
	{
		const std::size_t source_edge = index_distribution(random_engine, index_param_t(0, N));
		StopIdT operation_a_id        = another_run.at(source_edge).first;
		StopIdT operation_b_id        = another_run.at(source_edge).second;
		neighbour(operation_a_id, operation_b_id);
		assert(checkEdge(operation_a_id, operation_b_id));
	}

	void InstanceBasedSolutionGenerator::neighbour(StopIdT a_id, StopIdT b_id)
	{
		const auto iter_a = (a_id != start_stop_id ? ids.at(a_id) : run.getWorkStops( ).end( ));
		const auto iter_b = (b_id != start_stop_id ? ids.at(b_id) : run.getWorkStops( ).end( ));
		neighbour(iter_a, iter_b);
	}

	void InstanceBasedSolutionGenerator::neighbour(Run::WorkStopIterator a, Run::WorkStopIterator b)
	{
		switch(selectMutation(a, b))
		{
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
		assert(a != run.getWorkStops( ).end( ) || b != run.getWorkStops( ).end( ));

		if(b == run.getWorkStops( ).begin( ) && a == std::prev(run.getWorkStops( ).end( )))
		{
			return MutationType::VertexInsert;
		}

		if((a != run.getWorkStops( ).end( ) && std::next(a) == b) ||
		   (a == run.getWorkStops( ).end( ) && b == run.getWorkStops( ).begin( )))
		{
			return MutationType::None;
		}

		return selectRandomMutation( );
	}

	void InstanceBasedSolutionGenerator::addEdgeWithBlockInsert(Run::WorkStopIterator a, Run::WorkStopIterator b)
	{
		a                       = (a == run.getWorkStops( ).end( ) ? run.getWorkStops( ).begin( ) : std::next(a));
		const std::size_t a_idx = std::distance(run.getWorkStops( ).begin( ), a);
		const std::size_t b_idx = std::distance(run.getWorkStops( ).begin( ), b);

		if(a_idx < b_idx)
		{
			const std::size_t U = N - 1 - (b_idx - a_idx);
			std::size_t c_idx   = index_distribution(random_engine, index_param_t(0, U));
			if(c_idx >= a_idx)
			{
				c_idx += b_idx - a_idx + 1;
			}
			auto c = c_idx < a_idx ? std::prev(a, a_idx - c_idx) : std::next(b, c_idx - b_idx);

			blockInsert(a, b, c);
		}
		else
		{
			const std::size_t U     = (a_idx - b_idx) - 1;
			const std::size_t u     = U > 1 ? index_distribution(random_engine, index_param_t(0, U - 1)) : 0;
			const std::size_t c_idx = b_idx + u + 1;
			auto c                  = std::next(b, c_idx - b_idx);

			blockInsert(b, c, a);
		}
	}

	void InstanceBasedSolutionGenerator::addEdgeWithBlockReverse(Run::WorkStopIterator a, Run::WorkStopIterator b)
	{
		if(a == run.getWorkStops( ).end( ) ||
		   std::distance(run.getWorkStops( ).begin( ), a) < std::distance(run.getWorkStops( ).begin( ), b))
		{
			addEdgeWithBlockReverseDirect(a, b);
		}
		else
		{
			addEdgeWithBlockReverseCircular(a, b);
		}
	}

	void InstanceBasedSolutionGenerator::addEdgeWithBlockReverseDirect(Run::WorkStopIterator a, Run::WorkStopIterator b)
	{
		if(a == std::prev(run.getWorkStops( ).end( )) || b == run.getWorkStops( ).end( ))
		{
			blockReverse(a, b);
		}
		else if(b == run.getWorkStops( ).begin( ) || a == run.getWorkStops( ).end( ))
		{
			auto next_a = (a == run.getWorkStops( ).end( ) ? run.getWorkStops( ).begin( ) : std::next(a));
			blockReverse(next_a, std::next(b));
		}
		else
		{
			const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
			if(random_value < 0.5f)
			{
				blockReverse(std::next(a), std::next(b));
			}
			else
			{
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
		if(b == run.getWorkStops( ).end( ))
		{
			vertexInsert(a, b);
		}
		else if(a == run.getWorkStops( ).end( ))
		{
			vertexInsert(b, run.getWorkStops( ).begin( ));
		}
		else
		{
			const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
			if(random_value < 0.5f)
			{
				vertexInsert(b, std::next(a));
			}
			else
			{
				vertexInsert(a, b);
			}
		}
	}

	void InstanceBasedSolutionGenerator::addEdgeWithVertexSwap(Run::WorkStopIterator a, Run::WorkStopIterator b)
	{
		if(a == std::prev(run.getWorkStops( ).end( )) || b == run.getWorkStops( ).end( ))
		{
			vertexSwap(a, std::prev(b));
		}
		else if(b == run.getWorkStops( ).begin( ) || a == run.getWorkStops( ).end( ))
		{
			auto next_a = (a == run.getWorkStops( ).end( ) ? run.getWorkStops( ).begin( ) : std::next(a));
			vertexSwap(next_a, b);
		}
		else
		{
			const float random_value = float_distribution(random_engine, float_param_t(0.f, 1.f));
			if(random_value < 0.5f)
			{
				vertexSwap(std::next(a), b);
			}
			else
			{
				vertexSwap(a, std::prev(b));
			}
		}
	}

	void InstanceBasedSolutionGenerator::addInstance(const InstanceBasedSolutionGenerator& instance)
	{
		populations.push_back(&instance.edges);
	}

	bool InstanceBasedSolutionGenerator::checkEdge(StopIdT a_id, StopIdT b_id) const
	{
		bool result = false;
		if(a_id == start_stop_id)
		{
			result = run.getWorkStops( ).front( ).getOperation( ).getId( ) == b_id;
			if(!result)
			{
				LOG_ERROR(logger, "check edge first - {} failed", b_id);
			}
		}
		else if(b_id == start_stop_id)
		{
			result = run.getWorkStops( ).back( ).getOperation( ).getId( ) == a_id;
			if(!result)
			{
				LOG_ERROR(logger, "check edge {} - last failed", a_id);
			}
		}
		else
		{
			auto it = std::find_if(run.getWorkStops( ).begin( ),
			                       run.getWorkStops( ).end( ),
			                       [&](const WorkStop& workStop) {
				                       return workStop.getOperation( ).getId( ) == a_id;
				                   });
			result = (it != run.getWorkStops( ).end( ) &&
			          it != std::prev(run.getWorkStops( ).end( )) &&
			          std::next(it)->getOperation( ).getId( ) == b_id);
			if(!result)
			{
				LOG_ERROR(logger, "check edge {} - {} failed", a_id, b_id);
			}
		}
		return result;
	}

	void InstanceBasedSolutionGenerator::updateEdges( )
	{
		if(edges.empty( ))
		{
			edges.resize(run.getWorkStops( ).size( ) + 1);
		}
		std::size_t idx                                 = 0;
		InstanceBasedSolutionGenerator::StopIdT last_id = InstanceBasedSolutionGenerator::start_stop_id;
		for(auto& workStop : run.getWorkStops( ))
		{
			const InstanceBasedSolutionGenerator::StopIdT operation_id = workStop.getOperation( ).getId( );
			edges.at(idx++)                                            = std::make_pair(last_id, operation_id);
			last_id                                                    = operation_id;
		}
		edges.back( ) = std::make_pair(last_id, InstanceBasedSolutionGenerator::start_stop_id);
	}

	void InstanceBasedSolutionGenerator::store( )
	{
		SolutionGenerator::store( );
		updateEdges( );
	}
}
