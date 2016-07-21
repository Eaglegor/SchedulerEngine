#pragma once

#include <string>
#include <Engine/Concepts/Location.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/Concepts/TimeWindow.h>
#include <Engine/Concepts/Capacity.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "Order.h"
#include "Constraints/Operation/OperationConstraints.h"

#include <SceneManager_export.h>

namespace Scheduler
{
	/// Class representing single operation which needs to be done to complete the order
    class SCENEMANAGER_EXPORT Operation
    {
    public:
        Operation(size_t id);

        const char* getName() const;

        size_t getId() const;

        const Location& getLocation() const;

        const Duration& getDuration() const;

        const Order* getOrder() const;

        void setName(const char* name);

        void setLocation(const Location &location);

        void setDuration(const Duration &duration);

        void setOrder(const Order *order);

		const OperationConstraints& constraints() const;
		OperationConstraints& constraints();

	private:
        size_t id;
        std::string name;

        Location location;
        Duration duration;

        const Order* order;

		OperationConstraints operations_constraints;
    };
}