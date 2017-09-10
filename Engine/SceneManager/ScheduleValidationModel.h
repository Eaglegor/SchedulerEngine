#pragma once

#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>
#include <Engine/Utils/ReferenceWrapper.h>


namespace Scheduler
{
	/**
	 * @brief Algorithm used to detect the constraints violations
	 * 
	 * @details Rich VRP instances contain many different constraints. There are different contexts 
	 * of solution validation e.g. checking if there are any violations or printing out
	 * the list of violations. 
	 * 
	 * To simplify implementation of the new constraints, the validation of each
	 * constraint satisfaction is performed as a standalone algorithm. Instead of
	 * hard-coding these checks in the scene manager the validation model is used to
	 * provide the VRP and TSP solvers with them.
	 * 
	 * Validation model contains algorithms for schedule, run and stop validation.
	 * 
	 * @todo Validation of schedule should implicitly check runs validity
	 */
	class ScheduleValidationModel
	{
	public:
		/**
		 * @brief Default constructor
		 * 
		 * @details Initializes the model with empty algorithms (no constraints are checked)
		 */
		ScheduleValidationModel( );

		/**
		 * @brief Returns current stop validation algorithm
		 * 
		 * @return Current stop validation algorithm
		 */
		const StopValidationAlgorithm& getStopValidationAlgorithm( ) const;

		/**
		 * @brief Returns current run validation algorithm
		 * 
		 * @return Current run validation algorithm
		 */
		const RunValidationAlgorithm& getRunValidationAlgorithm( ) const;

		/**
		 * @brief Returns current schedule validation algorithm
		 * 
		 * @return Current schedule validation algorithm
		 */
		const ScheduleValidationAlgorithm& getScheduleValidationAlgorithm( ) const;

		/**
		 * @brief Sets current stop validation algorithm
		 * 
		 * @param algorithm New stop validation algorithm
		 */
		void setStopValidationAlgorithm(const StopValidationAlgorithm& algorithm);

		/**
		 * @brief Sets current run validation algorithm
		 * 
		 * @param algorithm New run validation algorithm
		 */
		void setRunValidationAlgorithm(const RunValidationAlgorithm& algorithm);

		/**
		 * @brief Sets current schedule validation algorithm
		 * 
		 * @param algorithm New schedule validation algorithm
		 */
		void setScheduleValidationAlgorithm(const ScheduleValidationAlgorithm& algorithm);

	private:
		ReferenceWrapper<const StopValidationAlgorithm> stop_validation_algorithm;
		ReferenceWrapper<const RunValidationAlgorithm> run_validation_algorithm;
		ReferenceWrapper<const ScheduleValidationAlgorithm> schedule_validation_algorithm;
	};
}