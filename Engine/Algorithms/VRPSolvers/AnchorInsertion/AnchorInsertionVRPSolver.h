#pragma once

#include <Engine/AlgorithmsManager/VRPSolver.h>
#include <Engine/Utils/Optional.h>
#include <AnchorInsertionVRPSolver.h>
#include <cstdint>
#include <memory>

namespace Scheduler
{
	class TSPSolver;
	class SceneCostFunction;

    class ANCHORINSERTIONVRPSOLVER_EXPORT AnchorInsertionVRPSolver : public VRPSolver
	{
	public:
        AnchorInsertionVRPSolver();
        ~AnchorInsertionVRPSolver();
		virtual void optimize(Scene& scene) const override;

		static constexpr const char* staticGetName() { return "AnchorInsertion"; }
		virtual const char* getName() const override { return staticGetName(); };
		
	private:
		Optional<const VRPSolver&> repair_algorithm;
		Optional<const SceneCostFunction&> cost_function;
		std::size_t iterations_limit;
	};
}
