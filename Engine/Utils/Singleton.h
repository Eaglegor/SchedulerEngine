#pragma once
#include <memory>

namespace Scheduler
{
	/** 
	 * @ingroup utils
	 * 
	 * @brief This class is a base for the singleton classes
	 * 
	 * @details Deriving from this class defines the basic singleton methods and the singleton instance.
	 * Static singleton instance pointer Singleton<T>::instance must be explicitly instantiated somewhere
	 * 
	 */
	template <typename T>
	class Singleton
	{
	public:
		virtual ~Singleton( ){};

		/** 
		 * @brief Creates the singleton instance if it wasn't yet created
		 * 
		 * @tparam Args Argument types of instance contructor
		 * @param args Arguments of instance constructor
		 * 
		 * @return Pointer to the created singleton or the pointer to the existing instance if it already exists
		 */
		template <typename... Args>
		static T* createInstance(Args&&... args)
		{
			assert(!instance);
			if(instance) return instance;
			instance = new T(std::forward<Args>(args)...);
			return instance;
		}

		/** 
		 * @brief Returns the pointer to the singleton instance
		 * 
		 * @return Pointer to the singleton instance or nullptr if it wasn't yet created
		 */
		static T* getInstance( )
		{
			assert(instance);
			return instance;
		}

		/** 
		 * @brief Destroys the singleton instance
		 */
		static void destroyInstance( )
		{
			assert(instance);
			instance = nullptr;
		}

	protected:
		Singleton( ) = default;

	private:
		static T* instance;
	};
}