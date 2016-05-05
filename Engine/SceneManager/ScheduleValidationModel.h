#pragma once

namespace Scheduler
{
	class StopValidationAlgorithm;
	class RunValidationAlgorithm;
	class ScheduleValidationAlgorithm;
	
	class ScheduleValidationModel
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
		
			void getStopValidationAlgorithm(StopValidationAlgorithm* algorithm);
			void getRunValidationAlgorithm(RunValidationAlgorithm* algorithm);
			void getScheduleValidationAlgorithm(ScheduleValidationAlgorithm* algorithm);
		
		private:
			StopValidationAlgorithm* stop_validation_algorithm;
			RunValidationAlgorithm* run_validation_algorithm;
			ScheduleValidationAlgorithm* schedule_validation_algorithm;
	};
}