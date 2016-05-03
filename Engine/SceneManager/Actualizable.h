#pragma once

namespace Scheduler
{
    class Stop;
    
    template<typename T, typename Actualizer>
    class Actualizable
    {
        public:
			Actualizable():
				is_actual(false)
			{}

            const T& get() const
            {
                if(!is_actual) actualizer.actualize();
                return value;
            }

            Actualizable& operator=(const T& value)
            {
                this->value = value;
				setActual(true);
				return *this;
            }
            
            void setActualizer(const Actualizer& actualizer)
            {
                this->actualizer = actualizer;
            }
            
			bool isActual() const
			{
				return is_actual;
			}

			void setActual(bool value)
			{
				this->is_actual = value;
			}

        private:
			bool is_actual;
            T value;
            Actualizer actualizer;
    };
}