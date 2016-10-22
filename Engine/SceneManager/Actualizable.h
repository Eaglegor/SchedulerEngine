#pragma once

#include <Engine/Utils/ReferenceWrapper.h>

namespace Scheduler
{
    class Stop;
    
    template<typename T, typename Actualizer>
    class Actualizable
    {
        public:
			explicit Actualizable(const Actualizer& actualizer):
			actualizer(actualizer)
			{}

            const T& get() const
            {
                actualizer.get().actualize();
                return value;
            }

            Actualizable& operator=(const T& value)
            {
                this->value = value;
				return *this;
            }
            
            void setActualizer(const Actualizer &actualizer)
            {
                this->actualizer = actualizer;
            }

        private:
            T value;
			
			ReferenceWrapper<const Actualizer> actualizer;
    };
}