#pragma once

#include <Engine/Utils/String.h>
#include <SceneManager_export.h>
#include <string>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT Attribute
	{
	public:
		Attribute(std::size_t id, const String& name);

		std::size_t getId( ) const;

		const String& getName( ) const;

		bool operator==(const Attribute& rhs) const;
		bool operator!=(const Attribute& rhs) const;

	private:
		std::size_t id;
		String name;
	};
}