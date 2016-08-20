#pragma once

#include "LinkedPointersList.h"

namespace Scheduler
{
	template<typename T, typename BackendCollection = LinkedPointersList<T>>
	class LinkedPointersSublist
	{
	public:
		static_assert(std::is_pointer<T>::value, "Type must be a pointer");
		static_assert(std::is_same<BackendCollection, LinkedPointersList<T>>::value || std::is_same<BackendCollection, LinkedPointersSublist<T>>::value, "Incompatible backend collection");
		
 		using value_type = typename BackendCollection::value_type;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator = typename BackendCollection::iterator;
		using const_iterator = typename BackendCollection::const_iterator;
		using reverse_iterator = typename BackendCollection::reverse_iterator;
		using const_reverse_iterator = typename BackendCollection::const_reverse_iterator;
		
		LinkedPointersSublist(const LinkedPointersSublist &rhs):
		parent(rhs.parent),
		parent_end(rhs.parent_end),
		head(rhs.head),
		tail(rhs.tail),
		_size(rhs._size)
		{
		}
		
		LinkedPointersSublist(BackendCollection &parent, iterator begin, iterator end):
		parent(parent),
		parent_end(end),
		head(*begin),
		tail(*std::prev(end)),
		_size(std::distance(begin, end))
		{}
		
		reference front()
		{
			return *begin();
		}
		
		const_reference front() const
		{
			return *begin();
		}
		
		reference back()
		{
			return *std::prev(end());
		}
		
		const_reference back() const
		{
			return *std::prev(end());
		}
		
		iterator begin() const
		{
			return iterator(head, &head, &tail, *parent_end);
		}
		
		const_iterator cbegin() const
		{
			return const_iterator(head, &head, &tail, *parent_end);
		}
		
		iterator end() const
		{
			return iterator(*parent_end, &head, &tail, *parent_end);
		}
		
		const_iterator cend() const
		{
			return const_iterator(*parent_end, &head, &tail, *parent_end);
		}
		
		reverse_iterator rbegin() const
		{
			return reverse_iterator(end());
		}
		
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(end());
		}
		
		reverse_iterator rend() const
		{
			return reverse_iterator(begin());
		}
		
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(begin());
		}
		
		bool empty() const
		{
			return _size == 0;
		}
		
		size_type size() const
		{
			return _size;
		}
		
		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max();
		}
		
		void clear()
		{
			if(empty()) return;
			tail = *parent.erase(begin(), end());
			head = tail;
			_size = 0;
		}
		
		iterator insert(iterator pos, value_type value)
		{
			if(pos == begin()) head = value;
			if(pos == end()) tail = value;
			iterator iter = parent.insert(pos, value);
			++_size;
			return iter;
		}
		
		iterator insert(iterator pos, iterator first, iterator last)
		{
			for(iterator iter = first; iter != last; ++iter)
			{
				insert(pos, *iter);
			}
		}
		
		iterator erase(iterator pos)
		{
			if(*pos == head) head = (*pos)->next();
			if(*pos == tail) tail = (*pos)->prev();
			
			iterator iter = parent.erase(pos);
			--_size;
			return iter;
		}
		
		iterator erase(iterator first, iterator last)
		{
			iterator current = first;
			while(current != last)
			{
				current = erase(current);
			}
			return last;
		}
		
		void push_back(value_type value)
		{
			insert(end(), value);
		}
		
		void pop_back()
		{
			erase(std::prev(end()));
		}

		void push_front(const_reference value)
		{
			insert(begin(), value);
		}
		
		void pop_front()
		{
			erase(begin());
		}
		
		void splice(iterator pos, LinkedPointersSublist<value_type, BackendCollection> &other, iterator first, iterator last)
		{
			if(&this->parent == &other.parent)
			{
				value_type new_head = head;
				value_type new_tail = tail;
				value_type pre_first = getPrevValue(first);
				value_type last_value = getPrevValue(last);
				value_type first_value = *first;
				
				if(first == begin()) new_head = *last;
				if(last == end()) new_tail = pre_first;
				if(pos == begin()) new_head = first_value;
				if(pos == end()) new_tail = last_value;
				
				parent.splice(pos, parent, first, last);
				
				head = new_head;
				tail = new_tail;
			}
			else
			{
				iterator current = first;
				while(current != last)
				{
					iterator next = std::next(current);
					other.erase(current);
					insert(pos, (*current));
					current = next;
				}
			}
		}
		
		void remove(value_type value)
		{
			remove_if([&value](value_type v){return v == value;});
		}
		
		template<typename UnaryPredicate>
		void remove_if(UnaryPredicate p)
		{
			iterator current = begin();
			while(current != end())
			{
				iterator next = std::next(current);
				if(p(*current)) erase(current);
				current = next;
			}			
		}
		
		void reverse()
		{
			auto i = begin();
			auto o = end();
			reverse(begin(), end());
		}
		
		void reverse(iterator first, iterator last)
		{
			value_type first_value = *first;
			value_type last_value = getPrevValue(last);
			parent.reverse(first, last);
			if(first == begin()) head = last_value;
			if(last == end()) tail = first_value;
		}
		
	private:
		value_type getPrevValue(iterator iter)
		{
			return iter == end() ? *std::prev(iter) : (*iter)->prev();
		}
		
		value_type getNextValue(iterator iter)
		{
			return *std::next(iter);
		}
		
		BackendCollection &parent;
		iterator parent_end;
		value_type head;
		value_type tail;
		size_type _size;
	};
}