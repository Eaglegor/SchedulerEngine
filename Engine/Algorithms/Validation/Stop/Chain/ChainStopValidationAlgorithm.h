#pragma once

#include <Engine/SceneManager/Extensions/StopValidationAlgorithm.h>
#include <ChainStopValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class CHAINSTOPVALIDATIONALGORITHM_EXPORT ChainStopValidationAlgorithm : public StopValidationAlgorithm
	{
	public:
		virtual bool isValid(const Stop * stop) const override;

		virtual void addAlgorithm(StopValidationAlgorithm* algorithm);

	private:
		std::list<StopValidationAlgorithm*> algorithms;
	};
}