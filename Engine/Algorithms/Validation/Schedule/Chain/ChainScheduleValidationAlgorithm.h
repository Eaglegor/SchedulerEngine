#pragma once

#include <Engine/SceneManager/Extensions/ScheduleValidationAlgorithm.h>
#include <ChainScheduleValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class CHAINSCHEDULEVALIDATIONALGORITHM_EXPORT ChainScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		virtual bool isValid(const Schedule * schedule) const override;

		virtual void addAlgorithm(ScheduleValidationAlgorithm* algorithm);

	private:
		std::list<ScheduleValidationAlgorithm*> algorithms;
	};
}