#pragma once

#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/trivial_value_traits.hpp>
#include <cstddef>
#include <Engine/Concepts/Route.h>
#include <Engine/Concepts/TimeWindow.h>
#include "Actualizable.h"
#include <functional>
#include <boost/intrusive/list.hpp>
#include <SceneManager_export.h>

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

	class SCENEMANAGER_EXPORT Stop
	{
	public:
		struct Context
		{
			DurationActualizer& duration_actualizer;
			ArrivalTimeActualizer& arrival_time_actualizer;
		};
		
		Stop(const Context& context, Run &run);
		Stop(const Stop& rhs) = delete;
		virtual ~Stop() {}

		void setStartTime(const TimePoint& time);
		void setEndTime(const TimePoint& time);

		const TimeWindow& getAllocationTime() const;
		void setAllocationTime(const TimeWindow &time);

		const Duration& getDuration() const;
		void setDuration(const Duration &duration);

		const Route& getNextRoute() const;
		void setNextRoute(const Route &route);

		const Run& getRun() const;
		Run& getRun();

		virtual const Location& getLocation() const = 0;
		
		virtual void acceptVisitor(StopVisitor& visitor) = 0;
		virtual void acceptVisitor(ConstStopVisitor& visitor) const = 0;

		bool isValid() const;
		void validate(ViolationsConsumer& violations_consumer) const;
		
	public:
		class IntrusiveListNode
		{
		public:
			IntrusiveListNode():stop(nullptr),next(nullptr),prev(nullptr){}
			explicit IntrusiveListNode(Stop* stop):stop(stop),next(nullptr),prev(nullptr){}
			
			void set_next(IntrusiveListNode* next){this->next = next; if(stop) stop->route_actualizer.setDirty(true);}
			IntrusiveListNode* get_next()const{return next;}
			void set_prev(IntrusiveListNode* prev){this->prev = prev;}
			IntrusiveListNode* get_prev()const{return prev;}
			
			Stop* getStop(){return stop;}
			const Stop* getStop() const{return stop;}
			
		private:
			Stop* stop;
			IntrusiveListNode* next;
			IntrusiveListNode* prev;
		};
		
	private:
		IntrusiveListNode list_node;
		
	public:
		
		struct IntrusiveNodeTraits
		{
			using node = IntrusiveListNode;
			using node_ptr = IntrusiveListNode*;
			using const_node_ptr = const IntrusiveListNode*;
			
			static node* get_next(const node* n) {return n->get_next();}
			static node* get_previous(const node* n) {return n->get_prev();}
			static void set_next(node* n, node* next) {n->set_next(next);}
			static void set_previous(node* n, node* prev) {n->set_prev(prev);}
		};
		
		struct IntrusiveValueTraits
		{
			using node_traits = IntrusiveNodeTraits;
			using node_ptr = node_traits::node_ptr;
			using const_node_ptr = node_traits::const_node_ptr;
			using value_type = Stop;
			using pointer = Stop*;
			using const_pointer = const Stop*;
			static const boost::intrusive::link_mode_type link_mode = boost::intrusive::normal_link;
			static node_ptr to_node_ptr(value_type &value){return node_ptr(&value.list_node);}
			static const_node_ptr to_node_ptr(const value_type &value){return const_node_ptr(&value.list_node);}
			static pointer to_value_ptr(node_ptr n){return pointer(n->getStop());}
			static const_pointer to_value_ptr(const_node_ptr n){return const_pointer(n->getStop());}
		};
		
		using list_t = boost::intrusive::list<Stop, boost::intrusive::value_traits<IntrusiveValueTraits>>;
		using iterator_type = list_t::iterator;
		using const_iterator_type = list_t::const_iterator;
		
	protected:
		Context context;
		RouteActualizer route_actualizer;
		
	private:
		using ActualizableAllocationTime = Actualizable<TimeWindow, ArrivalTimeActualizer>;
		using ActualizableDuration = Actualizable<Duration, DurationActualizer>;
		using ActualizableRoute = Actualizable<Route, RouteActualizer>;

		ActualizableAllocationTime allocation_time;
		ActualizableDuration duration;
		ActualizableRoute next_route;

		ReferenceWrapper<Run> run;

		friend class Run;
		friend class IntrusiveListNode;
	};
}