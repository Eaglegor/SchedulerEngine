#include "ScheduleValidationModel.h"

#include "Algorithms/Validation/StopValidationAlgorithm.h"
#include "Algorithms/Validation/RunValidationAlgorithm.h"
#include "Algorithms/Validation/ScheduleValidationAlgorithm.h"

namespace Scheduler
{
	class EmptyStopValidationAlgorithm : public StopValidationAlgorithm
	{
	public:
		virtual bool isValid(const Stop& stop) const override {return true;}
		virtual const char* getName() const override {return "Empty";}
	};
	
	class EmptyRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual bool isValid(const Run& run) const override {return true;}
		virtual const char* getName() const override {return "Empty";}
	};
	
	class EmptyScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		virtual bool isValid(const Schedule& schedule) const override {return true;}
		virtual const char* getName() const override {return "Empty";}
	};
	
	static EmptyStopValidationAlgorithm EMPTY_STOP_VALIDATION_ALGORITHM;
	static EmptyRunValidationAlgorithm EMPTY_RUN_VALIDATION_ALGORITHM;
	static EmptyScheduleValidationAlgorithm EMPTY_SCHEDULE_VALIDATION_ALGORITHM;
	
	ScheduleValidationModel::ScheduleValidationModel():
	stop_validation_algorithm(EMPTY_STOP_VALIDATION_ALGORITHM),
	run_validation_algorithm(EMPTY_RUN_VALIDATION_ALGORITHM),
	schedule_validation_algorithm(EMPTY_SCHEDULE_VALIDATION_ALGORITHM)
	{
	}
	
	const StopValidationAlgorithm& ScheduleValidationModel::getStopValidationAlgorithm() const
	{
		return stop_validation_algorithm;
	}
	
	const RunValidationAlgorithm& ScheduleValidationModel::getRunValidationAlgorithm() const
	{
		return run_validation_algorithm;
	}
	
	const ScheduleValidationAlgorithm& ScheduleValidationModel::getScheduleValidationAlgorithm() const
	{
		return schedule_validation_algorithm;
	}
	
	void ScheduleValidationModel::setStopValidationAlgorithm(const StopValidationAlgorithm &algorithm)
	{
		this->stop_validation_algorithm = algorithm;
	}
	
	void ScheduleValidationModel::setRunValidationAlgorithm(const RunValidationAlgorithm &algorithm)
	{
		this->run_validation_algorithm = algorithm;
	}
	
	void ScheduleValidationModel::setScheduleValidationAlgorithm(const ScheduleValidationAlgorithm &algorithm)
	{
		this->schedule_validation_algorithm = algorithm;
	}
}