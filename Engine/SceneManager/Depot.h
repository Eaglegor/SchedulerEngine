#pragma once

#include <Engine/Utils/String.h>

#include <cstddef>
#include <string>

namespace Scheduler
{
	class Location;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Represents the distribution base (warehouse, headquarter etc.)
	 * 
	 * @sa @ref scene_structure
	 */
	class Depot
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of the depot
		 * @param location Location of the depot
		 */
		Depot(std::size_t id, const Location& location);

		/**
		 * @brief Returns identifier of the depot
		 * 
		 * @return Idendifier of the depot
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns location of the depot
		 * 
		 * @return Location of the depot
		 */
		const Location& getLocation( ) const;

		/**
		 * @brief Returns name of the depot
		 * 
		 * @return Name of the depot
		 */
		const String& getName( ) const;

		/**
		 * @brief Sets name of the depot
		 * 
		 * @param name Name of the depot
		 */
		void setName(const String& name);

		/**
		 * @brief Equality operator
		 * 
		 * @param rhs Another depot
		 * 
		 * @return true, if rhs is the same object as this
		 */
		bool operator==(const Depot& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @param rhs Another depot
		 * 
		 * @return result of !(rhs == *this)
		 */
		bool operator!=(const Depot& rhs) const;

	private:
		std::size_t id;
		String name;
		const Location& location;
	};
}