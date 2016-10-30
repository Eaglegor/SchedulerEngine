#pragma once

#include <cstddef>
#include <string>
#include <Engine/Utils/String.h>
#include <SceneManager_export.h>

namespace Scheduler
{
	class Location;
	
	class SCENEMANAGER_EXPORT Depot
	{
	public:
		Depot(std::size_t id, const Location &location);
		
		std::size_t getId() const;
		const Location& getLocation() const;
		
		const String& getName() const;
		void setName(const String& name);

		bool operator==(const Depot& rhs) const;
		bool operator!=(const Depot& rhs) const;
		
	private:
		std::size_t id;
		String name;
		const Location& location;
	};
}