#pragma once

#include <utility>

namespace Scheduler
{
	class CallableVisitorProxy
	{
	public:
		template<typename VisitorType, typename ObjectType, typename... VisitorArgs>
		typename VisitorType::ReturnType call(ObjectType* type, VisitorArgs&&...visitor_args)
		{
			VisitorType visitor(std::forward<VisitorArgs>(visitor_args)...);
			type->acceptVisitor(&visitor);
			return visitor.getRetVal();
		}
	};
}