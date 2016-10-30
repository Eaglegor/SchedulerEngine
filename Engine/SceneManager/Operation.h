#pragma once

#include <string>
#include <Engine/Concepts/Site.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/Concepts/TimeWindow.h>
#include <Engine/Concepts/Capacity.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <boost/optional.hpp>
#include "Order.h"
#include "Constraints/Operation/OperationConstraints.h"
#include <Engine/Utils/Optional.h>

#include <SceneManager_export.h>

namespace Scheduler
{
	/// Class representing single operation which needs to be done to complete the order
    class SCENEMANAGER_EXPORT Operation
    {
    public:
        Operation(std::size_t id, const Location& location, Optional<const Order&> order);

        const String& getName() const;

        std::size_t getId() const;

        const Location& getLocation() const;

        const Duration& getDuration() const;

        Optional<const Order&> getOrder() const;

        void setName(const String& name);

        void setDuration(const Duration &duration);

		const OperationConstraints& constraints() const;
		OperationConstraints& constraints();

		bool operator==(const Operation& rhs) const;
		bool operator!=(const Operation& rhs) const;
		
	private:
        std::size_t id;
        String name;

        const Location& location;
        Duration duration;

        Optional<const Order&> order;

		OperationConstraints operations_constraints;
    };
}