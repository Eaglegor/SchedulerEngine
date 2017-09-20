#pragma once

namespace Scheduler
{
	/** 
	 * @ingroup utils
	 * 
	 * @brief This class performs the static_cast of base class pointer/reference to the derived class pointer/reference
	 * 
	 * @tparam Base Base class to cast from
	 * @tparam Derived Derived class to cast to
	 */
	template <typename Base, typename Derived>
	struct DerivedTypeCaster
	{
		/** 
		 * @brief Casts the base class reference to the derived class reference
		 * 
		 * @param value Reference to the base class object
		 * 
		 * @return Reference to the derived class object
		 */
		Derived& operator( )(Base& value) const
		{
			return static_cast<Derived&>(value);
		};

		/** 
		 * @brief Casts the const base class reference to the const derived class reference
		 * 
		 * @param value Reference to the const base class object
		 * 
		 * @return Reference to the const derived class object
		 */
		const Derived& operator( )(const Base& value) const
		{
			return static_cast<const Derived&>(value);
		};

		/** 
		 * @brief Casts the base class pointer to the derived class pointer
		 * 
		 * @param value Pointer to the base class object
		 * 
		 * @return Pointer to the derived class object
		 */
		Derived* operator( )(Base* value) const
		{
			return static_cast<Derived*>(value);
		};

		/** 
		 * @brief Casts the const base class pointer to the const derived class pointer
		 * 
		 * @param value Pointer to the const base class object
		 * 
		 * @return Pointer to the const derived class object
		 */
		const Derived* operator( )(const Base* value) const
		{
			return static_cast<const Derived*>(value);
		};
	};
}