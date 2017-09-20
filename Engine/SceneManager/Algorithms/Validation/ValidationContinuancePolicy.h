#pragma once

namespace Scheduler
{
	/**
	 * @ingroup constraints
	 * 
	 * @brief The policy used to determine if the validation process should continue
	 * 
	 * @details When the violations consumer consumes new violation it decides if it
	 * needs any other violations or not. For example if we need only to check the 
	 * fact of any violation existence we can stop on the first violation reported and
	 * spare the computational resources. After consuming the next violation, the consumer
	 * returns the desired policy and the validation algorithm uses this policy to
	 * understand if it has to break or not.
	 */
	enum class ValidationContinuancePolicy
	{
		/**
		 * @brief The value returned by the consumer to interrupt the validation algorithm
		 */
		INTERRUPT,

		/**
		 * @brief The value returned by the consumer to continue the validation algorithm
		 */
		CONTINUE
	};
}