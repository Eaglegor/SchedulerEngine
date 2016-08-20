#pragma once

#include <iterator>
#include <type_traits>
#include <vector>

namespace Scheduler
{
	template<typename CollectionType>
	class PositionPreservingLinkedPointersListWrapper
	{
	public:
		
		class Iterator : public std::iterator<std::bidirectional_iterator_tag, typename CollectionType::iterator>
		{
		public:
			using value_type = typename CollectionType::iterator;
			
			Iterator(){}
			
			explicit Iterator(typename std::vector<value_type>::iterator iter, typename std::vector<value_type>::iterator end, value_type end_value):
			iter(iter),
			end(end),
			end_value(end_value)
			{
			}
			
			Iterator(const Iterator& rhs):
			iter(rhs.iter),
			end(rhs.end),
			end_value(rhs.end_value)
			{
			}
			
			Iterator& operator=(const Iterator& rhs)
			{
				iter = rhs.iter;
				end = rhs.end;
				end_value = rhs.end_value;
				return *this;
			}
			
			Iterator& operator++()
			{
				++iter;
				return *this;
			}
			
			Iterator operator++(int)
			{
				Iterator i(*this);
				++(*this);
				return i;
			}
			
			Iterator& operator--()
			{
				--iter;
				return *this;
			}
			
			Iterator operator--(int)
			{
				Iterator i(*this);
				--(*this);
				return i;
			}
			
			bool operator==(const Iterator& rhs) const
			{
				return iter == rhs.iter;
			}
			
			bool operator!=(const Iterator& rhs) const
			{
				return !(*this == rhs);
			}
			
			const value_type operator*() const
			{
				if(iter == end) return end_value;
				return *iter;
			}
			
			value_type operator*()
			{
				if(iter == end) return end_value;
				return *iter;
			}
			
		private:
			typename std::vector<value_type>::iterator iter;
			typename std::vector<value_type>::iterator end;
			value_type end_value;
		};

		using value_type = typename CollectionType::iterator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator = Iterator;
		using const_iterator = const Iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		
		PositionPreservingLinkedPointersListWrapper(const CollectionType& collection):
		parent(collection)
		{
			for(auto iter = collection.begin(); iter != collection.end(); ++iter)
			{
				iterators.push_back(iter);
			}
		}
		
		iterator begin() 
		{
			return iterator(iterators.begin(), iterators.end(), parent.end());
		}
		
		const_iterator cbegin() const
		{
			return const_iterator(iterators.cbegin(), iterators.end(), parent.end());
		}
		
		iterator end() 
		{
			return iterator(iterators.end(), iterators.end(), parent.end());
		}
		
		const_iterator cend() const
		{
			return const_iterator(iterators.cend(), iterators.end(), parent.end());
		}
		
		reverse_iterator rbegin() 
		{
			return reverse_iterator(end());
		}
		
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(end());
		}
		
		reverse_iterator rend() 
		{
			return reverse_iterator(begin());
		}
		
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(begin());
		}
		
		bool empty() const
		{
			return iterators.empty();
		}
		
		size_type size() const
		{
			return iterators.size();
		}
		
		void update()
		{
			size_t index = 0;
			for(auto iter = parent.begin(); iter != parent.end(); ++iter, ++index)
			{
				iterators[index] = iter;
			}
		}
		
	private:
		const CollectionType& parent;
		std::vector<value_type> iterators;
	};
	
	
}