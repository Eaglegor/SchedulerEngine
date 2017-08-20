#pragma once

#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT ScheduleValidationModel
	{
	public:
		ScheduleValidationModel( );

		const StopValidationAlgorithm& getStopValidationAlgorithm( ) const;
		const RunValidationAlgorithm& getRunValidationAlgorithm( ) const;
		const ScheduleValidationAlgorithm& getScheduleValidationAlgorithm( ) const;

		void setStopValidationAlgorithm(const StopValidationAlgorithm& algorithm);
		void setRunValidationAlgorithm(const RunValidationAlgorithm& algorithm);
		void setScheduleValidationAlgorithm(const ScheduleValidationAlgorithm& algorithm);

	private:
		ReferenceWrapper<const StopValidationAlgorithm> stop_validation_algorithm;
		ReferenceWrapper<const RunValidationAlgorithm> run_validation_algorithm;
		ReferenceWrapper<const ScheduleValidationAlgorithm> schedule_validation_algorithm;
	};
}