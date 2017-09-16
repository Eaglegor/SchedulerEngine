#pragma once

#include "Actualizable.h"
#include <Engine/Concepts/Route.h>
#include <Engine/Concepts/TimeWindow.h>

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/trivial_value_traits.hpp>
#include <cstddef>
#include <functional>

#include "RouteActualizer.h"

#include "Location.h"
namespace Scheduler
{
	class ScheduleActualizationModel;
	class ScheduleValidationModel;
	class Run;
	class Operation;
	class RouteActualizationAlgorithm;
	class ArrivalTimeActualizationAlgorithm;
	class DurationActualizationAlgorithm;
	class Location;
	class ArrivalTimeActualizer;
	class DurationActualizer;
	class ViolationsConsumer;

	class StopVisitor;
	class ConstStopVisitor;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Planned operation performance (servicing order, loading at depot, unloading at depot)
	 */
	class Stop
	{
	public:
		/**
		 * @brief Internal stop context
		 */
		struct Context
		{
			/**
			 * @brief Actualizer used to update duration
			 */
			DurationActualizer& duration_actualizer;

			/**
			 * @brief Actualizer used to update arrival time
			 */
			ArrivalTimeActualizer& arrival_time_actualizer;
		};

		/**
		 * @brief Constructor
		 * 
		 * @param context Internal stop context
		 * @param run Run the stop belongs to
		 */
		Stop(const Context& context, Run& run);
		Stop(const Stop& rhs) = delete;
		virtual ~Stop( ){}

		/**
		 * @brief Sets the start time of the operation
		 * 
		 * @note This also updates the end time as @c startTime + @c duration
		 * 
		 * @param time New start time
		 */
		void setStartTime(const TimePoint& time);

		/**
		 * @brief Sets the end time of the operation
		 * 
		 * @note This also updates the start time as @c endTime - @c duration
		 * 
		 * @param time New end time
		 */
		void setEndTime(const TimePoint& time);

		/**
		 * @brief Returns the planned allocation time of the stop
		 * 
		 * @return Planned allocation time of the stop
		 */
		const TimeWindow& getAllocationTime( ) const;

		/**
		 * @brief Sets the planned allocation time of the stop
		 * 
		 * @param time New planned allocation time of the stop
		 */
		void setAllocationTime(const TimeWindow& time);

		/**
		 * @brief Returns the planned duration of servicing operation on the stop
		 * 
		 * @return Planned duration of servicing operation on the stop
		 */
		const Duration& getDuration( ) const;

		/**
		 * @brief Sets the planned duration of servicing operation on the stop
		 * 
		 * @param duration Planned duration of servicing operation on the stop
		 */
		void setDuration(const Duration& duration);

		/**
		 * @brief Returns the route to the next stop in the schedule
		 * 
		 * @return Route to the next stop in the schedule
		 */
		const Route& getNextRoute( ) const;

		/**
		 * @brief Sets the route to the next stop in the schedule
		 * 
		 * @param route Route to the next stop in the schedule
		 */
		void setNextRoute(const Route& route);

		/**
		 * @brief Returns the run the stop belongs to
		 * 
		 * @return Run the stop belongs to
		 */
		const Run& getRun( ) const;

		/**
		 * @brief Returns the run the stop belongs to
		 * 
		 * @return Run the stop belongs to
		 */
		Run& getRun( );

		/**
		 * @brief Returns the location of the stop
		 * 
		 * @return Location of the stop
		 */
		virtual const Location& getLocation( ) const = 0;

		/**
		 * @brief Accepts the stop visitor
		 * 
		 * @param Visitor to accept
		 */
		virtual void acceptVisitor(StopVisitor& visitor)            = 0;

		/**
		 * @brief Accepts the stop visitor
		 * 
		 * @param Visitor to accept
		 */
		virtual void acceptVisitor(ConstStopVisitor& visitor) const = 0;

		/**
		 * @brief Checks if the stop is valid in terms of the hard constraints satisfaction
		 * 
		 * @return @c true if no hard constraints are violated, @c false otherwise
		 */
		bool isValid( ) const;

		/**
		 * @brief Reports hard constraints violations to the specified consumer
		 * 
		 * @param violations_consumer Consumer that will process the reported violations
		 */
		void validate(ViolationsConsumer& violations_consumer) const;

	public:
		/**
		 * @brief The utilitary node class used to store the stop in the boost::intrusive list
		 */
		class IntrusiveListNode
		{
		public:
			IntrusiveListNode( )
			    : stop(nullptr), next(nullptr), prev(nullptr)
			{
			}
			explicit IntrusiveListNode(Stop* stop)
			    : stop(stop), next(nullptr), prev(nullptr)
			{
			}

			void set_next(IntrusiveListNode* next)
			{
				this->next = next;
				if(stop) stop->route_actualizer.setDirty(true);
			}
			IntrusiveListNode* get_next( ) const
			{
				return next;
			}
			void set_prev(IntrusiveListNode* prev)
			{
				this->prev = prev;
			}
			IntrusiveListNode* get_prev( ) const
			{
				return prev;
			}

			Stop* getStop( )
			{
				return stop;
			}
			const Stop* getStop( ) const
			{
				return stop;
			}

		private:
			Stop* stop;
			IntrusiveListNode* next;
			IntrusiveListNode* prev;
		};

	private:
		IntrusiveListNode list_node;

	public:
		/**
		 * @brief The utilitary node traits class used to store the stop in the boost::intrusive list
		 */
		struct IntrusiveNodeTraits
		{
			using node           = IntrusiveListNode;
			using node_ptr       = IntrusiveListNode*;
			using const_node_ptr = const IntrusiveListNode*;

			static node* get_next(const node* n)
			{
				return n->get_next( );
			}
			static node* get_previous(const node* n)
			{
				return n->get_prev( );
			}
			static void set_next(node* n, node* next)
			{
				n->set_next(next);
			}
			static void set_previous(node* n, node* prev)
			{
				n->set_prev(prev);
			}
		};

		/**
		 * @brief The utilitary value traits class used to store the stop in the boost::intrusive list
		 */
		struct IntrusiveValueTraits
		{
			using node_traits                                       = IntrusiveNodeTraits;
			using node_ptr                                          = node_traits::node_ptr;
			using const_node_ptr                                    = node_traits::const_node_ptr;
			using value_type                                        = Stop;
			using pointer                                           = Stop*;
			using const_pointer                                     = const Stop*;
			static const boost::intrusive::link_mode_type link_mode = boost::intrusive::normal_link;
			static node_ptr to_node_ptr(value_type& value)
			{
				return node_ptr(&value.list_node);
			}
			static const_node_ptr to_node_ptr(const value_type& value)
			{
				return const_node_ptr(&value.list_node);
			}
			static pointer to_value_ptr(node_ptr n)
			{
				return pointer(n->getStop( ));
			}
			static const_pointer to_value_ptr(const_node_ptr n)
			{
				return const_pointer(n->getStop( ));
			}
		};

		/**
		 * @brief Intrusive list of the stops
		 */
		using list_t              = boost::intrusive::list<Stop, boost::intrusive::value_traits<IntrusiveValueTraits>>;

		/**
		 * @brief Iterator of the intrusive stops list
		 */
		using iterator_type       = list_t::iterator;

		/**
		 * @brief Iterator of the intrusive stops list
		 */
		using const_iterator_type = list_t::const_iterator;

	protected:
		Context context;
		RouteActualizer route_actualizer;

	private:
		using ActualizableAllocationTime = Actualizable<TimeWindow, ArrivalTimeActualizer>;
		using ActualizableDuration       = Actualizable<Duration, DurationActualizer>;
		using ActualizableRoute          = Actualizable<Route, RouteActualizer>;

		ActualizableAllocationTime allocation_time;
		ActualizableDuration duration;
		ActualizableRoute next_route;

		ReferenceWrapper<Run> run;

		friend class Run;
		friend class IntrusiveListNode;
	};
}