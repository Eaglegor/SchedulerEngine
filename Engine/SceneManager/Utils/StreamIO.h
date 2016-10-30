#pragma once

#include <ostream>
#include <boost/optional/optional_io.hpp>
#include "../Vehicle.h"
#include "../Depot.h"
#include <SceneManager_export.h>

namespace Scheduler
{
	SCENEMANAGER_EXPORT std::ostream& operator<<(std::ostream& stream, const Vehicle& vehicle);;
	SCENEMANAGER_EXPORT std::ostream& operator<<(std::ostream& stream, const Depot& depot);
}