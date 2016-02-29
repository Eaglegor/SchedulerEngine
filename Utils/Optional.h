#pragma once

namespace Scheduler
{
	template<typename T>
	class Optional
	{
	public:
		Optional():
		value(nullptr)
		{}

		Optional& operator=(const T& value)
		{
			if(this->value) delete this->value;
			this->value = new T(value);
		}

		const T* get_pointer() const
		{
			return value;
		}

		T* get_pointer()
		{
			return value;
		}

		const T& get() const
		{
			return *value;
		}

		T& get()
		{
			return *value;
		}

		operator bool() const
		{
			return value != nullptr;
		}

	private:
		T *value;
	};
}