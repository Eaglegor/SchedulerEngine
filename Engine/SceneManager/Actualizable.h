#pragma once

namespace Scheduler
{
    class Stop;
    
    template<typename T, typename Actualizer>
    class Actualizable
    {
        public:
			Actualizable()
			{}

            const T& get() const
            {
                if(actualizer) actualizer->actualize();
                return value;
            }

            Actualizable& operator=(const T& value)
            {
                this->value = value;
				return *this;
            }
            
            void setActualizer(const Actualizer* actualizer)
            {
                this->actualizer = actualizer;
            }

        private:
            T value;
            const Actualizer* actualizer;
    };
}