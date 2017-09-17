#pragma once

#include <Engine/Concepts/Site.h>
#include <Engine/Utils/String.h>

#include <cstddef>
#include <string>

namespace Scheduler
{
	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Geographic location
	 * 
	 * @sa @ref scene_structure
	 */
	class Location
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of location
		 * @param site Site representing location coordinates
		 */
		Location(std::size_t id, const Site& site);

		/**
		 * @brief Returns location identifier
		 * 
		 * @return Location identifier
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns location coordinates
		 * 
		 * @return Site representing location coordinates
		 */
		const Site& getSite( ) const;

		/**
		 * @brief Returns name of the location
		 * 
		 * @return Name of the location
		 */
		const String& getName( ) const;

		/**
		 * @brief Sets name of the location
		 * 
		 * @param name Name of the location
		 */
		void setName(const String& name);

		/**
		 * @brief Equality operator
		 * 
		 * @return true, if rhs is the same object as this, false otherwise
		 */
		bool operator==(const Location& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @return result of !(*this == rhs)
		 */
		bool operator!=(const Location& rhs) const;

	private:
		std::size_t id;
		String name;
		Site site;
	};
}