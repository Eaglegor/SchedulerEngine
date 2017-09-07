#pragma once

#include <utility>

namespace Scheduler
{
	/**
	 * @ingroup utils
	 * 
	 * @brief This class incapsulates the call to the visitor and retrieving results of the visitor operation
	 * 
	 */
	class CallableVisitorProxy
	{
	public:
		/** 
		 * @brief Constructs the specified visitor, passes it to the object and returns the result of the operation store inside the visitor
		 * 
		 * @details The visitor type must meet the following requirements:
		 * 		- The type of operation result must be declared as VisitorType::ReturnType
		 * 		- The visitor must define the getRetVal() method returning VisitorType::ReturnType
		 * 
		 * The object must define the acceptVisitor(VisitorType&) method
		 * 
		 * When calling this method the following steps are performed:
		 * 		1. The visitor is created using the passed constructor arguments
		 * 		2. acceptVisitor(visitor) is called on the passed object
		 * 		3. visitor.getRetVal() is returned from this method
		 * 
		 * @tparam VisitorType The visitor type to pass to the object
		 * @tparam ObjectType The type of object to pass the visitor to
		 * @tparam VisitorArgs The types of the arguments passed to the visitor's constructor
		 * 
		 * @param type The object to pass the visitor to
		 * @param visitor_args The arguments to pass to the visitor's constructor
		 */
		template <typename VisitorType, typename ObjectType, typename... VisitorArgs>
		static typename VisitorType::ReturnType call(ObjectType& type, VisitorArgs&&... visitor_args)
		{
			VisitorType visitor(std::forward<VisitorArgs>(visitor_args)...);
			type.acceptVisitor(visitor);
			return visitor.getRetVal( );
		}
	};
}