#pragma once

#include <Engine/Utils/ReferenceWrapper.h>

namespace Scheduler
{
	class Stop;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Wraps an object and triggers actualization every time the value is requested
	 * 
	 * @tparam T Type of wrapped object
	 * @tparam Actualizer Type of the actualization functor
	 */
	template <typename T, typename Actualizer>
	class Actualizable
	{
	public:
		/**
		 * @brief Copy constructor
		 */
		explicit Actualizable(const Actualizer& actualizer)
		    : actualizer(actualizer)
		{
		}

		/**
		 * @brief Returns the value of wrapped object
		 * 
		 * @details Calls the actualizer actualize() method to perform value update 
		 * and then returns the reference to the wrapped object
		 * 
		 * @return Reference to the stored value
		 */
		const T& get( ) const
		{
			actualizer.get( ).actualize( );
			return value;
		}

		/**
		 * @brief Assignment operator
		 * 
		 * @param value New value to store
		 * 
		 * @return Reference to this
		 */
		Actualizable& operator=(const T& value)
		{
			this->value = value;
			return *this;
		}

		/**
		 * @brief Sets the actualization functor
		 * 
		 * @param actualizer The functor to be called when the value is requested
		 */
		void setActualizer(const Actualizer& actualizer)
		{
			this->actualizer = actualizer;
		}

	private:
		T value;

		ReferenceWrapper<const Actualizer> actualizer;
	};
}