#include "RunStopsView.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>

namespace Scheduler
{

	RunStopsView::RunStopsView(Run * run) :
		run(run),
		begin_iterator(run, 0),
		end_iterator(run, run->getWorkStops().size() + 2)
	{
	}

	const RunStopsView::iterator & RunStopsView::begin() const
	{
		return begin_iterator;
	}

	const RunStopsView::iterator & RunStopsView::end() const
	{
		return end_iterator;
	}

	size_t RunStopsView::size() const
	{
		return run->getWorkStops().size() + 2;
	}

	bool RunStopsView::empty() const
	{
		return false;
	}

	Stop * RunStopsView::operator[](size_t index)
	{
		return *(begin() + index);
	}

	RunStopsView::iterator::iterator(Run * run) :
		run(run),
		current_index(0)
	{
	}

	RunStopsView::iterator::iterator(Run * run, size_t current_index) :
		run(run),
		current_index(current_index)
	{
	}

	RunStopsView::iterator::iterator(const iterator & rhs) :
		run(rhs.run),
		current_index(rhs.current_index)
	{
	}

	Stop * RunStopsView::iterator::operator*()
	{
		if (current_index == 0) return run->getStartStop();
		if (current_index > run->getWorkStops().size()) return run->getEndStop();
		return run->getWorkStops()[current_index - 1];
	}

	RunStopsView::iterator & RunStopsView::iterator::operator=(const iterator & rhs)
	{
		this->run = rhs.run;
		this->current_index = rhs.current_index;
		return *this;
	}

	RunStopsView::iterator & RunStopsView::iterator::operator--()
	{
		--this->current_index;
		return *this;
	}

	RunStopsView::iterator & RunStopsView::iterator::operator++()
	{
		++this->current_index;
		return *this;
	}

	RunStopsView::iterator RunStopsView::iterator::operator--(int)
	{
		iterator iter(*this);
		--this->current_index;
		return iter;
	}

	RunStopsView::iterator RunStopsView::iterator::operator++(int)
	{
		iterator iter(*this);
		++this->current_index;
		return iter;
	}

	RunStopsView::iterator RunStopsView::iterator::operator+(size_t offset) const
	{
		return iterator(run, current_index + offset);
	}

	RunStopsView::iterator RunStopsView::iterator::operator-(size_t offset) const
	{
		return iterator(run, current_index - offset);
	}

	bool RunStopsView::iterator::operator==(const iterator & rhs)
	{
		return this->run == rhs.run && this->current_index == rhs.current_index;
	}

	bool RunStopsView::iterator::operator!=(const iterator & rhs)
	{
		return !(*this == rhs);
	}

	size_t RunStopsView::iterator::index() const
	{
		return current_index;
	}






	ConstRunStopsView::ConstRunStopsView(const Run * run) :
		run(run),
		begin_iterator(run, 0),
		end_iterator(run, run->getWorkStops().size() + 2)
	{
	}

	const ConstRunStopsView::iterator & ConstRunStopsView::begin() const
	{
		return begin_iterator;
	}

	const ConstRunStopsView::iterator & ConstRunStopsView::end() const
	{
		return end_iterator;
	}

	size_t ConstRunStopsView::size() const
	{
		return run->getWorkStops().size() + 2;
	}

	bool ConstRunStopsView::empty() const
	{
		return false;
	}

	const Stop * ConstRunStopsView::operator[](size_t index)
	{
		return *(begin() + index);
	}

	ConstRunStopsView::iterator::iterator(const Run * run) :
		run(run),
		current_index(0)
	{
	}

	ConstRunStopsView::iterator::iterator(const Run * run, size_t current_index) :
		run(run),
		current_index(current_index)
	{
	}

	ConstRunStopsView::iterator::iterator(const iterator & rhs) :
		run(rhs.run),
		current_index(rhs.current_index)
	{
	}

	const Stop * ConstRunStopsView::iterator::operator*()
	{
		if (current_index == 0) return run->getStartStop();
		if (current_index > run->getWorkStops().size()) return run->getEndStop();
		return run->getWorkStops()[current_index - 1];
	}

	ConstRunStopsView::iterator & ConstRunStopsView::iterator::operator=(const iterator & rhs)
	{
		this->run = rhs.run;
		this->current_index = rhs.current_index;
		return *this;
	}

	ConstRunStopsView::iterator & ConstRunStopsView::iterator::operator--()
	{
		--this->current_index;
		return *this;
	}

	ConstRunStopsView::iterator & ConstRunStopsView::iterator::operator++()
	{
		++this->current_index;
		return *this;
	}

	ConstRunStopsView::iterator ConstRunStopsView::iterator::operator--(int)
	{
		iterator iter(*this);
		--this->current_index;
		return iter;
	}

	ConstRunStopsView::iterator ConstRunStopsView::iterator::operator++(int)
	{
		iterator iter(*this);
		++this->current_index;
		return iter;
	}

	ConstRunStopsView::iterator ConstRunStopsView::iterator::operator+(size_t offset) const
	{
		return iterator(run, current_index + offset);
	}

	ConstRunStopsView::iterator ConstRunStopsView::iterator::operator-(size_t offset) const
	{
		return iterator(run, current_index - offset);
	}

	bool ConstRunStopsView::iterator::operator==(const iterator & rhs)
	{
		return this->run == rhs.run && this->current_index == rhs.current_index;
	}

	bool ConstRunStopsView::iterator::operator!=(const iterator & rhs)
	{
		return !(*this == rhs);
	}

	size_t ConstRunStopsView::iterator::index() const
	{
		return current_index;
	}
}