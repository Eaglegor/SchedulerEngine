#include "ScheduleValidationModel.h"

namespace Scheduler
{
	StopValidationAlgorithm * ScheduleValidationModel::getStopValidationAlgorithm() const
	{
		return stop_validation_algorithm;
	}
	RunValidationAlgorithm * ScheduleValidationModel::getRunValidationAlgorithm() const
	{
		return run_validation_algorithm;
	}
	ScheduleValidationAlgorithm * ScheduleValidationModel::getScheduleValidationAlgorithm() const
	{
		return schedule_validation_algorithm;
	}
	void ScheduleValidationModel::getStopValidationAlgorithm(StopValidationAlgorithm * algorithm)
	{
		this->stop_validation_algorithm = algorithm;
	}
	void ScheduleValidationModel::getRunValidationAlgorithm(RunValidationAlgorithm * algorithm)
	{
		this->run_validation_algorithm = algorithm;
	}
	void ScheduleValidationModel::getScheduleValidationAlgorithm(ScheduleValidationAlgorithm * algorithm)
	{
		this->schedule_validation_algorithm = algorithm;
	}
}