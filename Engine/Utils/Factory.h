#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

#include <assert.h>

#include <Engine/MemoryManager/MemoryManager.h>
#include <Engine/MemoryManager/PoolMemoryStore.h>

namespace Scheduler
{
	/** 
	 * @brief Templated factory class used to create the objects inside the scheduler engine
	 * 
	 * @details {Internally this factory uses pool allocator to allocate memory so the size of create objects is limited by a 
	 * fixed number passed on factory construction}
	 * 
	 * @tparam BaseClass {The type of base class for the objects created by this factory. 
	 * Base class and the classes derived from base class can be instantiated using this factory}
	 */
	template <typename BaseClass>
	class Factory
	{
	public:
		/** 
		 * @brief Constructor
		 * 
		 * @param max_object_size The maximal size of object that can be instantiated by this factory
		 * @param initial_capacity The initial capacity of the underlying pool memory allocator
		 * 
		 */
		Factory(size_t max_object_size, size_t initial_capacity = 10)
		    : memory_store(MemoryManager::createPoolMemoryStore(max_object_size, initial_capacity)),
		      max_object_size(max_object_size)
		{
			assert(initial_capacity > 0);
		}

		~Factory( )
		{
			MemoryManager::destroyMemoryStore(memory_store);
		}

		/** 
		 * @brief Creates the new object
		 * 
		 * @tparam DerivedClass The actual class, the object of which is constructed. Must be BaseClass or the class derived from it
		 * @tparam Args The types of arguments passed to the new object's constructor
		 * 
		 * @param args The arguments passed to the new object's constructor
		 * 
		 * @return Pointer to the created object or nullptr if sizeof(DerivedClass) is greater than max_object_size
		 */
		template <typename DerivedClass, typename... Args>
		DerivedClass* createObject(Args&&... args)
		{
			static_assert(std::is_base_of<BaseClass, DerivedClass>::value, "Requested object type is incompatible with factory base type");
			assert(sizeof(DerivedClass) <= max_object_size);
			if(sizeof(DerivedClass) > max_object_size) return nullptr;
			void* memory = memory_store->allocate(sizeof(DerivedClass));
			return new(memory) DerivedClass(std::forward<Args>(args)...);
		}

		/** 
		 * @brief Destroys the previously created object
		 * 
		 * @param object The object to be destoyed
		 * 
		 * @note Passing the object that wasn't created by this factory causes undefined behavior
		 */
		void destroyObject(BaseClass* object)
		{
			object->~BaseClass( );
			memory_store->deallocate(object);
		}

	private:
		size_t max_object_size;

		PoolMemoryStore* memory_store;
	};
}