#pragma once

#include <functional>
#include <algorithm>

namespace Scheduler
{
	template<typename BaseContainer>
	class Range
	{
	public:
		using base_container = BaseContainer;
		
		using iterator = typename base_container::iterator;
		using const_iterator = typename base_container::const_iterator;
		using reverse_iterator = typename base_container::reverse_iterator;
		using const_reverse_iterator = typename base_container::const_reverse_iterator;
		
		using value_type = typename base_container::value_type;
		using reference = typename base_container::reference;
		using const_reference = typename base_container::const_reference;
		
		Range():
		parent(nullptr),
		constant_time_size(0)
		{
		}
		
		Range(base_container* parent, iterator begin_iterator, iterator end_iterator):
		parent(parent),
		begin_iterator(begin_iterator),
		end_iterator(end_iterator),
		constant_time_size(std::distance(begin_iterator, end_iterator))
		{
		}
		
		Range(base_container* parent, iterator begin_iterator, iterator end_iterator, size_t n):
		parent(parent),
		begin_iterator(begin_iterator),
		end_iterator(end_iterator),
		constant_time_size(n)
		{
		}
		
		Range(Range&& rhs) :
			parent(rhs.parent),
			begin_iterator(rhs.begin_iterator),
			end_iterator(rhs.end_iterator),
			constant_time_size(rhs.constant_time_size)
		{
			rhs.parent = nullptr;
			rhs.begin_iterator = iterator();
			rhs.end_iterator = iterator();
			rhs.constant_time_size = 0;
		}

		Range& operator=(Range&& rhs)
		{
			parent = rhs.parent;
			begin_iterator = rhs.begin_iterator;
			end_iterator = rhs.end_iterator;
			constant_time_size = rhs.constant_time_size;
			
			rhs.parent = nullptr;
			rhs.begin_iterator = iterator();
			rhs.end_iterator = iterator();
			rhs.constant_time_size = 0;
			return *this;
		}
		
		const_reference front() const
		{
			return *begin_iterator;
		}
		
		reference front()
		{
			return *begin_iterator;
		}
		
		const_reference back() const
		{
			if(begin_iterator == end_iterator) return *end_iterator;
			return *std::prev(end_iterator);
		}
		
		reference back()
		{
			if(begin_iterator == end_iterator) return *end_iterator;
			return *std::prev(end_iterator);
		}
		
		iterator begin()
		{
			return begin_iterator;
		}
		
		const_iterator begin() const
		{
			return begin_iterator;
		}
		
		const_iterator cbegin() const
		{
			return begin_iterator;
		}
		
		iterator end()
		{
			return end_iterator;
		}
		
		const_iterator end() const
		{
			return end_iterator;
		}
		
		const_iterator cend() const
		{
			return end_iterator;
		}
		
		reverse_iterator rbegin()
		{
			return reverse_iterator(end_iterator);
		}
		
		const_reverse_iterator rbegin() const
		{
			return reverse_iterator(end_iterator);
		}
		
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(end_iterator);
		}
		
		reverse_iterator rend()
		{
			return reverse_iterator(begin_iterator);
		}
		
		const_reverse_iterator rend() const
		{
			return reverse_iterator(begin_iterator);
		}
		
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(begin_iterator);
		}
		
		bool empty() const
		{
			return begin_iterator == end_iterator;
		}
		
		size_t size() const
		{
			return constant_time_size;
		}
		
		void clear()
		{
			parent->erase(begin_iterator, end_iterator, constant_time_size);
			constant_time_size = 0;
			begin_iterator = end_iterator;
		}
		
		iterator insert(const_iterator pos, reference value)
		{
			return afterInsertionHook(pos, parent->insert(pos, value));
		}
		
		template<typename InputIterator>
		iterator insert(const_iterator pos, InputIterator first, InputIterator last)
		{
			parent->insert(pos, first, last);
			return afterInsertionHook(pos, first);
		}
		
		iterator erase(const_iterator pos)
		{
			beforeErasureHook(pos);
			return parent->erase(pos);
		}
		
		iterator erase(const_iterator first, const_iterator last)
		{
			beforeErasureHook(first, last, std::distance(first, last));
			return parent->erase(first, last);
		}
		
		iterator erase(const_iterator first, const_iterator last, size_t n)
		{
			beforeErasureHook(first, last, n);
			return parent->erase(first, last, n);
		}
		
		void push_back(reference value)
		{
			afterInsertionHook(end_iterator, parent->insert(end_iterator, value));
		}
		
		void push_front(reference value)
		{
			afterInsertionHook(begin_iterator, parent->insert(begin_iterator, value));
		}
		
		void pop_back()
		{
			beforeErasureHook(std::prev(end_iterator));
			parent->erase(std::prev(end_iterator));
		}
		
		void pop_front()
		{
			parent->erase(beforeErasureHook(begin_iterator));
		}
		
		void splice(const_iterator pos, Range& rhs)
		{
			if(this == &rhs) return;
			beforeSpliceHook(pos, rhs);
			parent->splice(pos, *rhs.parent);
		}
		
		void splice(const_iterator pos, Range& rhs, const_iterator new_element)
		{
			if(new_element == pos) return;
			beforeSpliceHook(pos, rhs, new_element);
			parent->splice(pos, *rhs.parent, new_element);
		}
		
		void splice(const_iterator pos, Range& rhs, const_iterator first, const_iterator last)
		{
			if(last == pos) return;
			beforeSpliceHook(pos, rhs, first, last);
			parent->splice(pos, *rhs.parent, first, last);
		}
		
		void splice(const_iterator pos, Range& rhs, const_iterator first, const_iterator last, size_t n)
		{
			if(last == pos) return;
			beforeSpliceHook(pos, rhs, first, last, n);
			parent->splice(pos, *rhs.parent, first, last, n);
		}
		
		void reverse()
		{
			reverse(begin_iterator, end_iterator);
		}
		
		void reverse(const_iterator first, const_iterator last)
		{
			if(first == last) return;
			while(std::prev(last) != first)
			{
				splice(first, *this, std::prev(last));
			}
		}
		
		void remove(const_reference value)
		{
			remove_if([&value](const_reference v){return v == value;});// std::bind1st(std::equal_to<const_reference>(), value));
		}
		
		template<typename Predicate>
		void remove_if(Predicate pred)
		{
			auto iter = begin_iterator;
			while(iter != end_iterator)
			{
				if(pred(*iter)) 
				{
					iter = erase(iter);
				}
				else ++iter;
			}
		}
		
		void setEnd(const_iterator new_end_iterator)
		{
			end_iterator = removeIteratorConstness(new_end_iterator);
			constant_time_size = std::distance(begin_iterator, end_iterator);
		}
		
		void setEnd(const_iterator new_end_iterator, size_t new_size)
		{
			end_iterator = removeIteratorConstness(new_end_iterator);
			constant_time_size = new_size;
		}
		
		template<typename Disposer>
		void erase_and_dispose(const_iterator first, const_iterator last, size_t n, Disposer disposer)
		{
			beforeErasureHook(first, last, n);
			parent->erase_and_dispose(first, last, disposer);
		}
		
		template<typename Disposer>
		void erase_and_dispose(const_iterator first, const_iterator last, Disposer disposer)
		{
			erase_and_dispose(first, last, std::distance(first, last), disposer);
		}
		
		template<typename Disposer>
		void clear_and_dispose(Disposer disposer)
		{
			erase_and_dispose(begin_iterator, end_iterator, size(), disposer);
		}
		
	private:
		
		iterator afterInsertionHook(const_iterator pos, iterator first_inserted)
		{
			if(begin_iterator == pos) begin_iterator = removeIteratorConstness(first_inserted);
			++constant_time_size;
			return first_inserted;
		}
		
		const_iterator beforeErasureHook(const_iterator pos)
		{
			auto next_iter = std::next(pos);
			if(begin_iterator == pos) begin_iterator = removeIteratorConstness(next_iter);
			--constant_time_size;
			return pos;
		}
		
		void beforeErasureHook(const_iterator first, const_iterator last, size_t n)
		{
			if(first == last) return;
			if(begin_iterator == first) begin_iterator = removeIteratorConstness(last);
			constant_time_size -= n;
		}
		
		void beforeSpliceHook(const_iterator pos, Range& range)
		{
			if(begin_iterator == pos) begin_iterator = removeIteratorConstness(range.begin());
			if(&range != this) {
				range.constant_time_size = 0;
				range.begin_iterator = range.end_iterator;
				constant_time_size+=range.size();
			}
		}
		
		void beforeSpliceHook(const_iterator pos, Range& range, const_iterator new_element)
		{
			bool rewrite_begin = false;
			if(begin_iterator == pos) rewrite_begin = true;
			if(new_element == range.begin_iterator) ++range.begin_iterator;
			if(&range != this) {
				++constant_time_size;
				--range.constant_time_size;
			}
			if(rewrite_begin) begin_iterator = removeIteratorConstness(new_element);
		}
		
		void beforeSpliceHook(const_iterator pos, Range& range, const_iterator first, const_iterator last)
		{
			beforeSpliceHook(pos, range, first, last, std::distance(first, last));
		}
		
		void beforeSpliceHook(const_iterator pos, Range& range, const_iterator first, const_iterator last, size_t n)
		{
			bool rewrite_begin = false;
			if(begin_iterator == pos) rewrite_begin = true;
			if(first == range.begin_iterator) range.begin_iterator = removeIteratorConstness(last);
			if(&range != this) {
				constant_time_size += n;
				range.constant_time_size -= n;
			}
			if(rewrite_begin) begin_iterator = removeIteratorConstness(first);
		}
		
		iterator removeIteratorConstness(const_iterator iter)
		{
			return erase(iter, iter);
		}
		
		size_t constant_time_size;
		iterator begin_iterator;
		iterator end_iterator;
		base_container* parent;
	};
}