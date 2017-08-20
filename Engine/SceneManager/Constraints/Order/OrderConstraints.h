#pragma once

#include <SceneManager_export.h>
#include "../ConstraintHolder.h"

namespace Scheduler
{
        class Depot;
    
	class SCENEMANAGER_EXPORT OrderConstraints
	{
	public:
            using DepotConstraint = ConstraintHolder<const Depot&>;
            
            const DepotConstraint& depot() const;
            DepotConstraint& depot();
            
	private:
            DepotConstraint depot_constraint;
	};
}
