#pragma once

#include "Algorithms/RunVehicleBinder.h"
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <assert.h>
#include <cstddef>
#include <unordered_set>

#include "SceneObjectsFactory.h"
#include <Engine/Utils/Factory.h>

#include "Constraints/Scene/SceneConstraints.h"
#include "Queries/SceneQueries.h"
#include "ScheduleActualizationModel.h"
#include <boost/optional.hpp>

#include "Listeners/StructuralChangesObserver.h"



namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;
	class SceneContext;
	class Performer;
	class SceneManager;
	class SceneQueries;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Solution of Vehicle Routing Problem
	 * 
	 * @details Scene contains schedules for the resources, i.e. a set
	 * of order assignments to the resources. Orders and resources themselves are defined
	 * by the scene context. Single scene context can have multiple associated scenes 
	 * (e.g. different solutions for the same set of input data).
	 * 
	 * @sa SceneContext
	 */
	class Scene
	{
	public:
		/**
		 * @brief List of the schedules in the scene
		 */
		using SchedulesList = std::vector<ReferenceWrapper<Schedule>>;

		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of scene
		 * @param context SceneContext for which the scene is defined
		 * @param scene_manager SceneManager that created the scene
		 */
		Scene(std::size_t id, const SceneContext& context, SceneManager& scene_manager);

		/**
		 * @brief Destructor
		 * 
		 * @details Destroys all schedules
		 */
		~Scene( );

		/**
		 * @brief Returns identifier of the scene
		 * 
		 * @return Identifier of the scene
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns the list of the schedules in the scene
		 * 
		 * @return List of the schedules in the scene
		 */
		const SchedulesList& getSchedules( ) const;

		/**
		 * @brief Returns the list of the schedules in the scene
		 * 
		 * @return List of the schedules in the scene
		 */
		SchedulesList& getSchedules( );

		/**
		 * @brief Creates new schedule for the specified performer
		 * 
		 * @param performer Performer to create the schedule for
		 * 
		 * @return Reference to the created schedule
		 */
		Schedule& createSchedule(const Performer& performer);

		/**
		 * @brief Returns context for which the scene was defined
		 * 
		 * @return Context for which the scene was defined
		 */
		const SceneContext& getContext( ) const;

		/**
		 * @brief Returns scene manager that created the scene
		 * 
		 * @return Scene manager that created the scene
		 */
		SceneManager& getSceneManager( ) const;

		/**
		 * @brief Checks if scene is valid in terms of hard constraints satisfaction
		 */
		bool isValid( ) const;

		/**
		 * @brief Adds structural changes listener
		 * 
		 * @param listener Structural changes listener
		 */
		void addStructuralChangesListener(StructuralChangesListener& listener);

		/**
		 * @brief Removes structural changes listener
		 * 
		 * @param listener Structural changes listener
		 */
		void removeStructuralChangesListener(StructuralChangesListener& listener);

		/**
		 * @brief Returns the interface to execure scene queries
		 * 
		 * @details Scene query is an interface to retrieve some kind of data from the scene
		 * (e.g. find work stop associated with the operation or calculate total vehicle demand)
		 * Queries can use caching to boost execution performance so it's the preferred way to retrieve
		 * supported information
		 * 
		 * @return Interface to execute scene queries
		 */
		const SceneQueries& query( ) const;

		/**
		 * @brief Equality operator
		 * 
		 * @param rhs Another scene
		 * 
		 * @return true, if rhs is the same object as this, false otherwise
		 */
		bool operator==(const Scene& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @param rhs Another scene
		 * 
		 * @return result of !(rhs == *this)
		 */
		bool operator!=(const Scene& rhs) const;

	private:
		std::size_t id;

		const SceneContext& context;

		SchedulesList schedules;

		SceneObjectsFactory<Schedule> schedules_factory;
		SceneObjectsFactory<Run> runs_factory;
		SceneObjectsFactory<WorkStop> stops_factory;

		StructuralChangesObserver structural_changes_observer;

		SceneManager& scene_manager;

		mutable Optional<SceneQueries> scene_queries;
	};
}
