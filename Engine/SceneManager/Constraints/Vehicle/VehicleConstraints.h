#pragma once
#include <memory>

namespace Scheduler
{
	class Capacity;
	
	class VehicleConstraints
	{
		public:
			const Capacity* capacity() const;
			
			void addConstraint(const Capacity& constraint);
			void removeCapacityConstraint();
			
		private:
			std::unique_ptr<Capacity> capacity_constraint;
	};
}