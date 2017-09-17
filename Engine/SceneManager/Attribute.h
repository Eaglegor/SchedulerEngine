#pragma once

#include <Engine/Utils/String.h>

#include <string>

namespace Scheduler
{
	/**
	 * @ingroup scene_manager
	 * 
	 * @brief String-based object representing attribute of vehicle or performer
	 * 
	 * @details Attribute can represent driver/performer skills, equipment of vehicle or other resource feature
	 * 
	 * @sa @ref scene_structure
	 */
	class Attribute
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of attribute
		 * @param name The name of attribute
		 */
		Attribute(std::size_t id, const String& name);

		/**
		 * @brief Returns the identifier of attribute
		 * 
		 * @return Identifier of attribute
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns the name of attribute
		 * 
		 * @return Name of attribute
		 */
		const String& getName( ) const;

		/**
		 * @brief Equality operator
		 * 
		 * @param rhs Another attribute
		 * 
		 * @return true, if rhs is the same object as this, false otherwise
		 */
		bool operator==(const Attribute& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @param rhs Another attribute
		 * 
		 * @return result of !(rhs == *this)
		 */
		bool operator!=(const Attribute& rhs) const;

	private:
		std::size_t id;
		String name;
	};
}