#include "StopIterator.h"
#include "Stop.h"

namespace Scheduler {
	
	StopIterator::StopIterator(Stop* stop) :current_stop(stop)
	{
	}

	StopIterator::StopIterator(const StopIterator& rhs):current_stop(rhs.current_stop)
	{
	}

	StopIterator::StopIterator(StopIterator&& rhs):current_stop(rhs.current_stop)
	{
		rhs.current_stop = nullptr;
	}

	StopIterator StopIterator::operator++(int)
	{
		StopIterator iter(*this);
		this->current_stop = this->current_stop-> getNextStop();
		return iter;
	}

	StopIterator& StopIterator::operator++()
	{
		this->current_stop = this->current_stop->getNextStop();
		return *this;
	}

	StopIterator StopIterator::operator--(int)
	{
		StopIterator iter(*this);
		this->current_stop = this->current_stop->getPrevStop();
		return iter;
	}
	
	StopIterator& StopIterator::operator--()
	{
		this->current_stop = this->current_stop->getPrevStop();
		return *this;
	}
	
	Stop* StopIterator::operator*() const
	{
		return current_stop;
	}

	Stop* StopIterator::operator->() const
	{
		return current_stop;
	}
	
	bool StopIterator::operator!=(const StopIterator& iter) const
	{
		return !(*this == iter);
	}

	bool StopIterator::operator==(const StopIterator& iter) const
	{
		return this->current_stop == iter.current_stop;
	}
	
}