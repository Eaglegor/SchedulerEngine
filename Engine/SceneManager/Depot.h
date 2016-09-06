#pragma once

#include <cstddef>
#include <string>
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
		
		void setName(const char* name);
		const char* getName() const;
		
	private:
		std::size_t id;
		std::string name;
		const Location& location;
	};
}