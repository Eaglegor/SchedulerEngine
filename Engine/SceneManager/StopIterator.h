#pragma once

namespace Scheduler
{
	class Stop;
	
	class StopIterator
	{
	public:
		StopIterator(Stop* stop);
		StopIterator(const StopIterator& rhs);
		StopIterator(StopIterator&& rhs);
		
		StopIterator& operator++();
		StopIterator operator++(int);
		
		StopIterator& operator--();
		StopIterator operator--(int);

		bool operator==(const StopIterator& iter) const;
		bool operator!=(const StopIterator& iter) const;

		Stop* operator*() const;
		Stop* operator->() const;
		
	private:
		Stop* current_stop;
	};
}