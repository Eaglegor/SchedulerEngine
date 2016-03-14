add_library(SchedulerEngine SHARED

		Concepts/Basic/Capacity.h
        Concepts/Basic/Capacity.cpp
		
		Concepts/Basic/Coordinate.h
        Concepts/Basic/Coordinate.cpp
		
		Concepts/Basic/Cost.h
        Concepts/Basic/Cost.cpp
		
		Concepts/Basic/Distance.h
        Concepts/Basic/Distance.cpp
		
		Concepts/Basic/Duration.h
        Concepts/Basic/Duration.cpp
		
		Concepts/Basic/Location.h
        Concepts/Basic/Location.cpp
		
		Concepts/Basic/Route.h
        Concepts/Basic/Route.cpp
		
		Concepts/Basic/RoutingProfile.h
        Concepts/Basic/RoutingProfile.cpp
		
		Concepts/Basic/Speed.h
        Concepts/Basic/Speed.cpp
		
		Concepts/Basic/SpeedDurationDistanceOperations.h
        Concepts/Basic/SpeedDurationDistanceOperations.cpp
		
		Concepts/Basic/TimePoint.h
        Concepts/Basic/TimePoint.cpp
		
		Concepts/Basic/TimeWindow.h
        Concepts/Basic/TimeWindow.cpp

		SceneManager/Attribute.h
        SceneManager/Attribute.cpp
		
        SceneManager/Operation.h
		SceneManager/Operation.cpp
		
		SceneManager/Order.h
        SceneManager/Order.cpp
		
		SceneManager/Performer.h
        SceneManager/Performer.cpp
		
		SceneManager/Run.h
        SceneManager/Run.cpp
		
		SceneManager/Stop.h
        SceneManager/Stop.cpp
		
		SceneManager/Scene.h
        SceneManager/Scene.cpp
		
		SceneManager/SceneManager.h
        SceneManager/SceneManager.cpp
		
		SceneManager/Schedule.h
        SceneManager/Schedule.cpp
		
		SceneManager/Vehicle.h
        SceneManager/Vehicle.cpp

		SceneManager/Views/RunStopsView.h
        SceneManager/Views/RunStopsView.cpp
		
        SceneManager/Views/ScheduleStopsView.h
		SceneManager/Views/ScheduleStopsView.cpp

		SceneManager/ScheduleActualization/TimeWindowSelector.h
		
		SceneManager/ScheduleActualization/TimeWindowSelectors/FirstTimeWindowSelector.h
        SceneManager/ScheduleActualization/TimeWindowSelectors/FirstTimeWindowSelector.cpp

		SceneManager/RunVehicleBinding/RunVehicleBinder.h
		
		SceneManager/RunVehicleBinding/PerformerAssignedVehicleBinder.h
        SceneManager/RunVehicleBinding/PerformerAssignedVehicleBinder.cpp

		SceneManager/ScheduleActualization/ScheduleActualizer.h
        SceneManager/ScheduleActualization/ScheduleActualizer.cpp
		
		SceneManager/ScheduleActualization/ScheduleActualizationAlgorithm.h
		SceneManager/ScheduleActualization/ScheduleActualizationAlgorithmType.h
		
		SceneManager/ScheduleActualization/Algorithms/StopArrivalTimeActualizationAlgorithm.h
        SceneManager/ScheduleActualization/Algorithms/StopArrivalTimeActualizationAlgorithm.cpp
		
		SceneManager/ScheduleActualization/Algorithms/StopDurationActualizationAlgorithm.h
        SceneManager/ScheduleActualization/Algorithms/StopDurationActualizationAlgorithm.cpp
        )
		
include(GenerateExportHeader)
generate_export_header(SchedulerEngine EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SchedulerEngine_export.h)
