#pragma once

#include <ostream>
#include <Engine/Concepts/Duration.h>
#include <Engine/Concepts/TimePoint.h>
#include <Engine/Concepts/TimeWindow.h>

namespace Scheduler {
    std::ostream &operator<<(std::ostream &stream, const TimePoint &value) {
        stream << "TimePoint(" << value.getValue() << ")";
        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, const TimeWindow &value) {
        stream << "TimeWindow(" << value.getStartTime() << " - " << value.getEndTime() << ")";
        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, const Duration &value) {
        stream << "Duration(" << value.getValue() << ")";
        return stream;
    }
}
