#pragma once

#include <cstddef>
#include <string>
#include <Engine/Concepts/Site.h>
#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT Location
	{
	public:
		Location(std::size_t id, const Site& site);
		
		std::size_t getId() const;
		const Site& getSite() const;
		
		const char* getName() const;
		void setName(const char* name);
		
		bool operator==(const Location& rhs) const;
		bool operator!=(const Location& rhs) const;
		
	private:
		std::size_t id;
		std::string name;
		Site site;
	};
}