#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <CWSavingsVRPSolver_export.h>

namespace Scheduler
{
	template<typename Saving>
	class CWSAVINGSVRPSOLVER_EXPORT VectorSavingsStore
	{
	public:
		using iterator = typename std::vector<Saving>::iterator;
		using const_iterator = typename std::vector<Saving>::const_iterator;
		using comparator = std::function< bool (const Saving&, const Saving&) >;
		using value_type = Saving;
		
		VectorSavingsStore():
		less_function(std::greater<Saving>()),
		sorted(false)
		{
		}
		
		template<typename... Args>
		void emplace_back(Args&& ...args)
		{
			sorted = false;
			savings.emplace_back(std::forward<Args>(args)...);
		}
		
		void push_back(const Saving& s)
		{
			sorted = false;
			savings.push_back(s);
		}
		
		iterator insert(iterator pos, const Saving& s)
		{
			sorted = false;
			return savings.insert(pos, s);
		}
		
		template<typename InputIterator>
		iterator insert(iterator pos, InputIterator first, InputIterator last)
		{
			sorted = false;
			return savings.insert(pos, first, last);
		}
		
		iterator erase(iterator pos)
		{
			sorted = false;
			return savings.erase(pos);
		}
		
		void setComparator(comparator comp)
		{
			less_function = comp;
			sorted = false;
		}
		
		iterator begin()
		{
			if(!sorted) std::sort(savings.begin(), savings.end(), less_function);
			return savings.begin();
		}
		
		const_iterator begin() const
		{
			if(!sorted) std::sort(savings.begin(), savings.end(), less_function);
			return savings.begin();
		}
		
		iterator end()
		{
			if(!sorted) std::sort(savings.begin(), savings.end(), less_function);
			return savings.end();
		}
		
		const_iterator end() const
		{
			if(!sorted) std::sort(savings.begin(), savings.end(), less_function);
			return savings.end();
		}
		
		size_t size() const
		{
			return savings.size();
		}
		
	private:
		bool sorted;
		std::vector<Saving> savings;
		comparator less_function;
	};
}