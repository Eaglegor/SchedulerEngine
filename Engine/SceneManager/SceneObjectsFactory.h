#pragma once

#include <Engine/Utils/Factory.h>
#include <cstddef>

namespace Scheduler
{
	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Abstract factory able to create scene objects
	 * 
	 * @details Each scene object has to have identifier unique for it's type.
	 * This factory takes the responsibility of passing unique identifiers
	 * to the objects constructors. The actual objects creation is done by using
	 * nested Factory<T> instance.
	 * 
	 * @tparam T Type of objects to create
	 */
	template <typename T>
	class SceneObjectsFactory
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param initial_capacity Count of objects to preallocate the memory for
		 */
		explicit SceneObjectsFactory(std::size_t initial_capacity = 10)
		    : counter(0),
		      backend_factory(sizeof(T), initial_capacity)
		{
		}

		/**
		 * @brief Creates the new scene object
		 * 
		 * @tparam Args Types of arguments to pass to the object's constructor
		 * 
		 * @param args Arguments to pass to the object's constructor
		 * 
		 * @return Pointer to the created object
		 */
		template <typename... Args>
		T* createObject(Args&&... args)
		{
			return backend_factory.template createObject<T>(counter++, std::forward<Args>(args)...);
		}

		/**
		 * @brief Destroys the previously created object
		 * 
		 * @warning The object must be created by this factory. Passing the object
		 * not created by this factory causes undefined behavior
		 * 
		 * @param object Object to destroy
		 */
		void destroyObject(T* object)
		{
			backend_factory.destroyObject(object);
		}

	private:
		std::size_t counter;

		Factory<T> backend_factory;
	};
}