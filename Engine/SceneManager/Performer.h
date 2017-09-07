#pragma once

#include <Engine/Concepts/Cost.h>
#include <Engine/Concepts/TimeWindow.h>
#include <cstddef>
#include <string>

#include "Attribute.h"
#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <Engine/Utils/String.h>

#include "Constraints/Performer/PerformerConstraints.h"



namespace Scheduler
{
	/**
	 * @ingroup scene_manager
	 * 
	 * @brief A person who serves customers
	 * 
	 * @details The resource concept is divided into 2 parts in this engine: performer and vehicle.
	 * Performer has the associated schedule (plan of actions for a workday) and may use different vehicles during his workday.
	 */
	class Performer
	{
	public:
		/**
		 * @brief Type representing performer skill
		 */
		using Skill     = Attribute;

		/**
		 * @brief Type representing performer's set of skills
		 */
		using SkillsSet = std::unordered_set<ReferenceWrapper<const Skill>>;

		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of performer
		 */
		Performer(std::size_t id);

		/**
		 * @brief Returns identifier of the performer
		 * 
		 * @return Identifier of the performer
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns name of the performer
		 * 
		 * @return Name of the performer
		 */
		const String& getName( ) const;

		/**
		 * @brief Sets name of the performer
		 * 
		 * @param name Name of the performer
		 */
		void setName(const String& name);

		/**
		 * @brief Returns cost of a single duration unit of the performer's work
		 * 
		 * @note Duration unit is defined by the Duration class
		 * 
		 * @return Cost of a single duration unit of performer's work
		 */
		const Cost& getDurationUnitCost( ) const;

		/**
		 * @brief Sets cost of a single duration unit of the performer's work
		 * 
		 * @note Duration unit is defined by the Duration class
		 * 
		 * @param cost Cost of a single duration unit of performer's work
		 */
		void setDurationUnitCost(const Cost& cost);

		/**
		 * @brief Returns cost of using performer's work
		 * 
		 * @details This is a cost of the fact of using performer during the day. It can represent e.g. the
		 * driver's regular salary or penalties defined in the cost functions to make the scheduler to
		 * reduce the count of used resources
		 * 
		 * @return Cost of using performer's work
		 */
		const Cost& getActivationCost( ) const;

		/**
		 * @brief Sets cost of using performer's work
		 * 
		 * @details This is a cost of the fact of using performer during the day. It can represent e.g. the
		 * driver's regular salary or penalties defined in the cost functions to make the scheduler to
		 * reduce the count of used resources
		 * 
		 * @param cost Cost of using performer's work
		 */
		void setActivationCost(const Cost& cost);

		/**
		 * @brief Returns the set of skills of the performer
		 */
		const SkillsSet& getSkills( ) const;

		/**
		 * @brief Adds the skill to the performer
		 * 
		 * @param skill Skill to add to the performer
		 */
		void addSkill(const Skill& skill);

		/**
		 * @brief Removes the skill from the performer
		 * 
		 * @param skill Skill to remove from the performer
		 */
		void removeSkill(const Skill& skill);

		/**
		 * @brief Clears performer's skills
		 */
		void clearSkills( );

		/**
		 * @brief Returns reference to the performer constraints
		 * 
		 * @return Reference to the performer constraints
		 */
		const PerformerConstraints& constraints( ) const;

		/**
		 * @brief Returns reference to the performer constraints
		 * 
		 * @return Reference to the performer constraints
		 */
		PerformerConstraints& constraints( );

		/**
		 * @brief Equality operator
		 * 
		 * @param rhs Another performer
		 * 
		 * @return true, if rhs is the same object as this
		 */
		bool operator==(const Performer& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @param rhs Another performer
		 * 
		 * @return result of !(rhs == *this)
		 */
		bool operator!=(const Performer& rhs) const;

	private:
		std::size_t id;
		String name;

		Cost duration_unit_cost;
		Cost activation_cost;

		SkillsSet skills;

		PerformerConstraints performer_constraints;
	};
}
