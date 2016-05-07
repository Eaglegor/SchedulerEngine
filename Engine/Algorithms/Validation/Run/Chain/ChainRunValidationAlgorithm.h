#pragma once

#include <Engine/SceneManager/Extensions/RunValidationAlgorithm.h>
#include <ChainRunValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class CHAINRUNVALIDATIONALGORITHM_EXPORT ChainRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual bool isValid(const Run * run) const override;

		virtual void addAlgorithm(RunValidationAlgorithm* algorithm);

	private:
		std::list<RunValidationAlgorithm*> algorithms;
	};
}