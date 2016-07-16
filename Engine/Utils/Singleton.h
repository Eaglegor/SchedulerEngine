#pragma once
#include <memory>

namespace Scheduler
{
	template<typename T>
	class Singleton
	{
	public:
		virtual ~Singleton(){};

		template<typename... Args>
		static T* createInstance(Args&&...args)
		{
			assert(!instance);
			if (instance) return instance;
			instance = new T(std::forward<Args>(args)...);
			return instance;
		}

		static T* getInstance()
		{
			assert(instance);
			return instance;
		}

		static void destroyInstance()
		{
			assert(instance);
			instance = nullptr;
		}

	protected:
		Singleton() = default;

	private:
		static T* instance;
	};
}