#pragma once

#include "Stop.h"
#include <Engine/Utils/Collections/RangeTypeAdapter.h>

#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <SceneManager_export.h>

namespace Scheduler
{
	class WorkOperation;

	class SCENEMANAGER_EXPORT WorkStop : public Stop
	{
	public:
		WorkStop(std::size_t id, const Stop::Context& context, Run& run, const WorkOperation& operation);
		WorkStop(const WorkStop& rhs) = delete;

		std::size_t getId( );

		const WorkOperation& getOperation( ) const;

		virtual const Location& getLocation( ) const override;

		void acceptVisitor(StopVisitor& visitor) override;
		void acceptVisitor(ConstStopVisitor& visitor) const override;

		bool operator==(const WorkStop& rhs) const;
		bool operator!=(const WorkStop& rhs) const;

	private:
		std::size_t id;
		const WorkOperation& operation;
	};
}
