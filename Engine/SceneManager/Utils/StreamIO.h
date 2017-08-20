#pragma once

#include "../Depot.h"
#include "../Vehicle.h"
#include <SceneManager_export.h>
#include <boost/optional/optional_io.hpp>
#include <ostream>

namespace Scheduler
{
	SCENEMANAGER_EXPORT std::ostream& operator<<(std::ostream& stream, const Vehicle& vehicle);
	;
	SCENEMANAGER_EXPORT std::ostream& operator<<(std::ostream& stream, const Depot& depot);
}