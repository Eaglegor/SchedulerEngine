#pragma once

#include "../Run.h"
#include <functional>
#include <vector>

#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT InvariantWorkStopsList
	{
	public:
		class Iterator : public std::iterator<std::random_access_iterator_tag, const WorkStop>
		{
		public:
			Iterator(const std::vector<ReferenceWrapper<const WorkStop>>* collection, std::size_t index):
			collection(collection),
			index(index)
			{}
			
			Iterator(const Iterator& rhs):
			collection(rhs.collection),
			index(rhs.index)
			{}
			
			Iterator& operator=(const Iterator& rhs)
			{
				collection = rhs.collection;
				index = rhs.index;
				return *this;
			}
			
			bool operator==(const Iterator &rhs) const
			{
				return collection == rhs.collection && index == rhs.index;
			}
			
			bool operator!=(const Iterator &rhs) const
			{
				return !(*this == rhs);
			}
			
			Iterator& operator++()
			{
				++index;
				return *this;
			}
			
			Iterator operator++(int)
			{
				Iterator it(*this);
				++index;
				return it;
			}
			
			Iterator& operator--()
			{
				--index;
				return *this;
			}
			
			Iterator operator--(int)
			{
				Iterator it(*this);
				--index;
				return it;
			}
			
			Iterator operator+(difference_type n)
			{
				return Iterator(collection, index + n);
			}
			
			Iterator operator-(difference_type n)
			{
				return Iterator(collection, index - n);
			}
			
			Iterator& operator+=(difference_type n)
			{
				index += n;
				return *this;
			}
			
			Iterator& operator-=(difference_type n)
			{
				index -= n;
				return *this;
			}
			
			const WorkStop& operator*()
			{
				return collection->at(index);
			}
			
			const WorkStop* operator->()
			{
				return &(collection->at(index).get());
			}
			
			difference_type operator-(const Iterator &rhs)
			{
				return index - rhs.index;
			}
			
		private:
			std::size_t index;
			const std::vector<ReferenceWrapper<const WorkStop>>* collection;
		};
		
		using iterator = Iterator;
		using const_iterator = Iterator;
		
		InvariantWorkStopsList() = default;
		InvariantWorkStopsList(Run::WorkStopsList &stops);

		InvariantWorkStopsList& operator=(InvariantWorkStopsList &&rhs);
		InvariantWorkStopsList& operator=(const Run::WorkStopsList &rhs);
		
		const_iterator begin() const;
		const_iterator end() const;
		
		Run::ConstWorkStopIterator getRunWorkStopIterator(const Run& r, const_iterator iter);
		
	private:
		std::vector<ReferenceWrapper<const WorkStop>> work_stops;
	};
}