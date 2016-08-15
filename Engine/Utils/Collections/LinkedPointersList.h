#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <limits>
#include <iostream>

namespace Scheduler
{
	template<typename T>
	class LinkedPointersList
	{
	public:
		
		static_assert(std::is_pointer<T>::value, "Type must be a pointer");
	
		class Iterator : public std::iterator<std::bidirectional_iterator_tag, T>
		{
		public:
			Iterator(T prev, T current, T next):prev(prev), current(current), next(next)
			{
			}
			
			Iterator(const Iterator& rhs):prev(rhs.prev), current(rhs.current), next(rhs.next)
			{
			}
			
			Iterator& operator=(const Iterator& rhs)
			{
				this->prev = rhs.prev;
				this->current = rhs.current;
				this->next = rhs.next;
			}
			
			Iterator& operator++()
			{
				prev = current;
				current = next;
				next = (next == nullptr ? nullptr : next->next());
				return *this;
			}
			
			Iterator operator++(int)
			{
				Iterator iter(prev, current, next);
				prev = current;
				current = next;
				next = (next == nullptr ? nullptr : next->next());
				return iter;
			}
			
			Iterator& operator--()
			{
				next = current;
				current = prev;
				prev = (prev == nullptr ? nullptr : prev->prev());
				return *this;
			}
			
			Iterator operator--(int)
			{
				Iterator iter(prev, current, next);
				next = current;
				current = prev;
				prev = (prev == nullptr ? nullptr : prev->prev());
				return iter;
			}
			
			bool operator==(const Iterator& rhs) const
			{
				return prev == rhs.prev && current == rhs.current && next == rhs.next;
			}
			
			bool operator!=(const Iterator& rhs) const
			{
				return !(*this == rhs);
			}
			
			const T& operator*() const
			{
				return current;
			}
			
			T& operator*()
			{
				return current;
			}
			
			const T operator->() const
			{
				return current;
			}
			
			T operator->()
			{
				return current;
			}
			
		private:
			T prev;
			T current;
			T next;
		};
		
		using value_type = T;
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
		
		LinkedPointersList():
		head(nullptr),
		tail(head),
		_size(0)
		{}
		
		reference front()
		{
			return head;
		}
		
		const_reference front() const
		{
			return head;
		}
		
		reference back()
		{
			return tail;
		}
		
		const_reference back() const
		{
			return tail;
		}
		
		iterator begin()
		{
			return iterator(nullptr, head, head == nullptr ? nullptr : head->next());
		}
		
		const_iterator cbegin() const
		{
			return const_iterator(nullptr, head, head == nullptr ? nullptr : head->next());
		}
		
		iterator end()
		{
			return iterator(tail, tail == nullptr ? nullptr : tail->next(), nullptr);
		}
		
		const_iterator cend() const
		{
			return const_iterator(tail, tail == nullptr ? nullptr : tail->next(), nullptr);
		}
		
		reverse_iterator rbegin()
		{
			return reverse_iterator(std::prev(end()));
		}
		
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(std::prev(end()));
		}
		
		reverse_iterator rend()
		{
			return reverse_iterator(std::prev(begin()));
		}
		
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(std::prev(begin()));
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
			head = nullptr;
			tail = head;
			_size = 0;
		}
		
		iterator insert(iterator pos, value_type value)
		{
			if(pos == end())
			{
				value->setNext(nullptr);
				if(empty())
				{
					head = value;
					value->setPrev(nullptr);
				}
				else
				{
					value_type prev = tail;
					prev->setNext(value);
					value->setPrev(prev);
				}
				tail = value;
				++_size;
			}
			else
			{
				if(pos == begin()) head = value;
				value_type prev = pos->prev();
				value_type next = *pos;
				if(prev != nullptr) prev->setNext(value);
				value->setPrev(prev);
				value->setNext(next);
				next->setPrev(value);
				++_size;
			}
			return iterator(value->prev(), value, value->next());
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
			value_type prev = pos->prev();
			value_type value = *pos;
			value_type next = pos->next();
			
			if(value == tail) tail = value->prev();
			if(value == head) head = value->next();
			
			if(prev != nullptr) prev->setNext(next);
			if(next != nullptr) next->setPrev(prev);
			pos->setNext(nullptr);
			pos->setPrev(nullptr);

			--_size;
			
			return iterator(prev, next, next == nullptr ? nullptr : next->next());
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
		
		void splice(iterator pos, LinkedPointersList<value_type> &other, iterator first, iterator last)
		{
			if(this == &other)
			{
				value_type old_prev = first->prev();
				value_type old_next = last->next();
				value_type new_prev = pos->prev();
				value_type new_next = pos;
				if(old_prev != nullptr) old_prev->setNext(old_next);
				if(old_next != nullptr) old_next->setPrev(old_prev);
				
				first->setPrev(new_prev);
				if(new_prev != nullptr) new_prev->setNext(*first);
				
				last->setNext(new_next);
				if(new_next != nullptr) new_next->setPrev(*last);
				
				if(pos == begin()) head = *first;
				if(pos == std::prev(end())) tail = *last;
			}
			else
			{
				iterator current = first;
				while(current != last)
				{
					iterator next = std::next(current);
					other->erase(current);
					insert(pos, current);
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
			for(iterator iter = begin(); iter != end(); ++iter)
			{
				value_type prev = iter->getPrev();
				value_type next = iter->getNext();
				iter->setPrev(next);
				iter->setNext(prev);
			}
		}
		
		void sort()
		{}
		
		template<typename Compare>
		void sort(Compare comp){}
	
	private:
		value_type head;
		value_type tail;
		
		size_type _size;
	};
}