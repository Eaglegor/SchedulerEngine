#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <cstdint>
#include <Engine/LoggingManager/LoggingManager.h>
#include <RotateTSPSolver_export.h>

namespace Scheduler
{
	class ROTATETSPSOLVER_EXPORT RotateTSPSolver : public TSPSolver
	{
	public:
		RotateTSPSolver();
		
		virtual const char* getName() const;
		static const char* staticGetName();
		
		virtual void optimize(Run& run) const;
		virtual void optimize(Schedule& schedule) const;
		
		void setRatio(float ratio);
		
	private:
		float ratio;
		Logger& logger;
	};
}