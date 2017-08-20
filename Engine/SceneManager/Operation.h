#pragma once

#include "Constraints/Operation/OperationConstraints.h"
#include "Order.h"
#include <Engine/Concepts/Capacity.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/Concepts/Site.h>
#include <Engine/Concepts/TimeWindow.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Optional.h>
#include <boost/optional.hpp>
#include <string>

#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT Operation
	{
	public:
		Operation(std::size_t id, const Order& order);
		virtual ~Operation( )
		{
		}

		const String& getName( ) const;

		std::size_t getId( ) const;

		const Location& getLocation( ) const;

		const Duration& getDuration( ) const;

		const Order& getOrder( ) const;

		void setName(const String& name);

		void setDuration(const Duration& duration);

		const OperationConstraints& constraints( ) const;
		OperationConstraints& constraints( );

		bool operator==(const Operation& rhs) const;
		bool operator!=(const Operation& rhs) const;

	private:
		std::size_t id;
		String name;

		Duration duration;

		const Order& order;

		OperationConstraints operation_constraints;
	};
}
