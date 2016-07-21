#pragma once

#include <memory>
#include <assert.h>

namespace Scheduler
{
    template<typename T>
    class ConstraintHolder
    {
        public:
			void set(const T& constraint)
			{
				this->constraint = std::make_shared<T>(constraint);
			}

			const T& get() const
			{
				assert(constraint);
				return *constraint;
			}

			operator const T&() const
			{
				return *constraint;
			}

			bool isSet() const 
			{
				return constraint != nullptr;
			}

		private:
			std::shared_ptr<T> constraint;
	};
}