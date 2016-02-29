#pragma once

#include <string>
#include <Engine/Concepts/Basic/Location.h>
#include <Engine/Concepts/Basic/Duration.h>
#include <Engine/Concepts/Basic/TimeWindow.h>
#include <Utils/Collections/ImmutableVector.h>
#include <Engine/Concepts/Basic/Capacity.h>
#include "Order.h"

namespace Scheduler
{
    class Operation
    {
    public:
        Operation(size_t id);

        const char* getName() const;

        size_t getId() const;

        ImmutableVector<TimeWindow>& getTimeWindows() const;

        const Location& getLocation() const;

        const Duration& getDuration() const;

        const Order* getOrder() const;

        void setName(const char* name);

        void setTimeWindows(ImmutableVector<TimeWindow> &time_windows);

        void setLocation(const Location &location);

        void setDuration(const Duration &duration);

        void setOrder(const Order *order);

		const Capacity& getLoad() const;

		void setLoad(const Capacity &load);

	private:
        size_t id;
        std::string name;

        std::vector<TimeWindow> time_windows;
        Location location;
        Duration duration;

        const Order* order;
		Capacity load;
    };
}