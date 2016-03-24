#pragma once

#include "Stop.h"

#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT WorkStop : public Stop
	{
	public:
		WorkStop(size_t id, const Location& location, Run* run);

		size_t getId();

		const Operation* getOperation() const;
		void setOperation(const Operation* operation);

		void acceptVisitor(StopVisitor* visitor) override;

	private:
		size_t id;
		const Operation* operation;
	};
}