add_library(SchedulerEngine SHARED

        Concepts/Basic/Capacity.cpp
        Concepts/Basic/Coordinate.cpp
        Concepts/Basic/Cost.cpp
        Concepts/Basic/Distance.cpp
        Concepts/Basic/Duration.cpp
        Concepts/Basic/Location.cpp
        Concepts/Basic/Route.cpp
        Concepts/Basic/RoutingProfile.cpp
        Concepts/Basic/Speed.cpp
        Concepts/Basic/SpeedDurationDistanceOperations.cpp
        Concepts/Basic/TimePoint.cpp
        Concepts/Basic/TimeWindow.cpp

        SceneManager/Attribute.cpp
        SceneManager/Operation.cpp
        SceneManager/Order.cpp
        SceneManager/Performer.cpp
        SceneManager/Run.cpp
        SceneManager/Stop.cpp
        SceneManager/Scene.cpp
        SceneManager/SceneManager.cpp
        SceneManager/Schedule.cpp
        SceneManager/Vehicle.cpp

        SceneManager/Views/RunStopsView.cpp
        )