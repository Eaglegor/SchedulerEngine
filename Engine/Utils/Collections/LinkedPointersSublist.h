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
		
		LinkedPointersSublist(BackendCollection &parent, iterator begin, iterator end):
		_parent(parent),
		_head(*begin),
		_tail(*std::prev(end)),
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
		
		iterator begin()
		{
			return iterator(_head == nullptr ? nullptr : _head->prev(), _head, _head == nullptr ? nullptr : _head->next());
		}
		
		const_iterator cbegin() const
		{
			return const_iterator(_head == nullptr ? nullptr : _head->prev(), _head, _head == nullptr ? nullptr : _head->next());
		}
		
		iterator end()
		{
			return iterator(_tail, _tail == nullptr ? nullptr : _tail->next(), (_tail == nullptr || _tail->next() == nullptr) ? nullptr : _tail->next()->next());
		}
		
		const_iterator cend() const
		{
			return const_iterator(_tail, _tail == nullptr ? nullptr : _tail->next(), (_tail == nullptr || _tail->next() == nullptr) ? nullptr : _tail->next()->next());
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
			_tail = *_parent.erase(begin(), end());
			_head = _tail;
			_size = 0;
		}
		
		iterator insert(iterator pos, value_type value)
		{
			if(pos == begin()) _head = value;
			if(pos == end()) _tail = value;
			iterator iter = _parent.insert(pos, value);
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
			if(*pos == _head) _head = (*pos)->next();
			if(*pos == _tail) _tail = (*pos)->prev();
			
			iterator iter = _parent.erase(pos);
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
		
		void splice(iterator pos, LinkedPointersSublist<value_type> &other, iterator first, iterator last)
		{
			if(&this->_parent == &other._parent)
			{
				if(pos == begin()) _head = *first;
				if(pos == end()) _tail = *std::prev(last);
				_parent.splice(pos, _parent, first, last);
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
			reverse(begin(), end());
		}
		
		void reverse(iterator first, iterator last)
		{
			if(first == begin()) _head = *std::prev(last);
			if(last == end()) _tail = *first;
			_parent.reverse(first, last);
		}
		
	private:
		BackendCollection &_parent;
		value_type _head;
		value_type _tail;
		size_type _size;
	};
}