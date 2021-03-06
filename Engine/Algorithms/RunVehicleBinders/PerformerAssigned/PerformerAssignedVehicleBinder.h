#pragma once

#include <Engine/SceneManager/Algorithms/RunVehicleBinder.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <unordered_map>



namespace Scheduler
{
	class Performer;
	class Vehicle;
	class Scene;

	class PerformerAssignedVehicleBinder : public RunVehicleBinder
	{
	public:
		PerformerAssignedVehicleBinder( );

		virtual bool bindVehicle(Run& run) const override;

		constexpr static const char* staticGetName( )
		{
			return "PerformerAssigned";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}

		void assign(const Performer& performer, const Vehicle& vehicle);
		void unassign(const Performer& performer, const Vehicle& vehicle);

	private:
		std::unordered_map<ReferenceWrapper<const Performer>, ReferenceWrapper<const Vehicle>> bindings;
	};
}
