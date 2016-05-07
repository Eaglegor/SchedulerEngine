#pragma once

#include <SceneManager_export.h>

namespace Scheduler
{
	class StopValidationAlgorithm;
	class RunValidationAlgorithm;
	class ScheduleValidationAlgorithm;
	
	class SCENEMANAGER_EXPORT ScheduleValidationModel
	{
		public:
			ScheduleValidationModel():
			stop_validation_algorithm(nullptr),
			run_validation_algorithm(nullptr),
			schedule_validation_algorithm(nullptr)
			{}
		
			StopValidationAlgorithm* getStopValidationAlgorithm() const;
			RunValidationAlgorithm* getRunValidationAlgorithm() const;
			ScheduleValidationAlgorithm* getScheduleValidationAlgorithm() const;
		
			void setStopValidationAlgorithm(StopValidationAlgorithm* algorithm);
			void setRunValidationAlgorithm(RunValidationAlgorithm* algorithm);
			void setScheduleValidationAlgorithm(ScheduleValidationAlgorithm* algorithm);
		
		private:
			StopValidationAlgorithm* stop_validation_algorithm;
			RunValidationAlgorithm* run_validation_algorithm;
			ScheduleValidationAlgorithm* schedule_validation_algorithm;
	};
}