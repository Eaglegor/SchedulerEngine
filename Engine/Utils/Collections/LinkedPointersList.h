#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <limits>

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
			using value_type = T;
			
			Iterator():
				current(nullptr),
				tail(nullptr),
				head(nullptr)
			{
			}
			
			Iterator(value_type current, const value_type* head, const value_type* tail):
				current(current),
				head(head),
				tail(tail)
			{
			}
			
			Iterator(const Iterator& rhs):
				current(rhs.current),
				head(rhs.head),
				tail(rhs.tail)
			{
			}
			
			Iterator(Iterator &&rhs) :
				current(rhs.current),
				head(rhs.head),
				tail(rhs.tail)
			{
				rhs.current = nullptr;
				rhs.head = nullptr;
				rhs.tail = nullptr;
			}

			Iterator& operator=(const Iterator& rhs)
			{
				this->current = rhs.current;
				this->tail = rhs.tail;
				this->head = rhs.head;
				return* this;
			}
			
			Iterator& operator++()
			{
				if (current == nullptr) current = *head;
				else if (current == *tail) current = nullptr;
				else current = current->next();
				return *this;
			}
			
			Iterator operator++(int)
			{
				Iterator iter(*this);
				++(*this);
				return iter;
			}
			
			Iterator& operator--()
			{
				if (current == nullptr) current = *tail;
				else if (current == *head) current = nullptr;
				else current = current->prev();
				return *this;
			}
			
			Iterator operator--(int)
			{
				Iterator iter(*this);
				--(*this);
				return iter;
			}
			
			bool operator==(const Iterator& rhs) const
			{
				return current == rhs.current;
			}
			
			bool operator!=(const Iterator& rhs) const
			{
				return !(*this == rhs);
			}
			
			const value_type& operator*() const
			{
				return current;
			}
			
			value_type& operator*()
			{
				return current;
			}
			
			const value_type* operator->() const
			{
				return &current;
			}
			
			value_type* operator->()
			{
				return &current;
			}
			
		private:
			value_type current;
			const value_type* head;
			const value_type* tail;
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
		
		iterator begin() const
		{
			return iterator(head, &head, &tail);
		}
		
		const_iterator cbegin() const
		{
			return const_iterator(head, &head, &tail);
		}
		
		iterator end() const
		{
			return iterator(nullptr, &head, &tail);
		}
		
		const_iterator cend() const
		{
			return const_iterator(nullptr, &head, &tail);
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
				value_type prev = (*pos)->prev();
				value_type next = *pos;
				if(prev != nullptr) prev->setNext(value);
				value->setPrev(prev);
				value->setNext(next);
				next->setPrev(value);
				++_size;
			}
			return iterator(value, &head, &tail);
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
			value_type prev = (*pos)->prev();
			value_type value = *pos;
			value_type next = (*pos)->next();
			
			if(value == tail) tail = value->prev();
			if(value == head) head = value->next();
			
			if(prev != nullptr) prev->setNext(next);
			if(next != nullptr) next->setPrev(prev);
			(*pos)->setNext(nullptr);
			(*pos)->setPrev(nullptr);

			--_size;
			
			return iterator(next, &head, &tail);
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
		
		void splice(iterator pos, LinkedPointersList<value_type> &other, iterator first, iterator last)
		{
			if(this == &other)
			{
				value_type old_prev = *std::prev(first);
				value_type old_next = *last;
				value_type new_prev = *std::prev(pos);
				value_type new_next = *pos;
				if(old_prev != nullptr) old_prev->setNext(old_next);
				if(old_next != nullptr) old_next->setPrev(old_prev);
				
				(*first)->setPrev(new_prev);
				if(new_prev != nullptr) new_prev->setNext(*first);
				
				(*std::prev(last))->setNext(new_next);
				if(new_next != nullptr) new_next->setPrev(*std::prev(last));
				
				if(pos == begin()) head = *first;
				if(pos == end()) tail = *std::prev(last);
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
			iterator iter = first;
			while(iter != last)
			{
				iterator next_iter = std::next(iter);
				value_type prev = (*iter)->prev();
				value_type next = (*iter)->next();
				(*iter)->setPrev(next);
				(*iter)->setNext(prev);
				iter = next_iter;
			}
			if (first == begin()) head = *std::prev(last);
			if (last == end()) tail = *first;
		}
	
	private:
		value_type head;
		value_type tail;
		
		size_type _size;
	};
}
