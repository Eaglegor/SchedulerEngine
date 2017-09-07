#pragma once

#include <algorithm>
#include <functional>

namespace Scheduler
{
	/** 
	 * @brief Represents range between 2 iterators in the container
	 * 
	 * @details Range is a part of the container limited by 2 iterators. It behaves like the 
	 * standalone container and all structural changes (insertions, removals) are automatically forwarded 
	 * to the underlying container. Since range itself behaves as another container, nested ranges may be introduced where the 
	 * changes are forwarded in the cascade way.
	 * 
	 * @warning Since the structure of the underlying container isn't known the size() operation can't be performed 
	 * in constant time using only the iterators. The size variable is used to avoid the slow linear operations.
	 * This variable is updated on all modifying operations made through the range interface. But the direct modifications
	 * of the underlying container aren't mirrored in the range and may leave it in the inconsistend state
	 * 	 
	 */
	template <typename BaseContainer>
	class Range
	{
	public:
		/// The type of the base container
		using base_container = BaseContainer;

		/// Iterator type
		using iterator               = typename base_container::iterator;

		/// Const iterator type
		using const_iterator         = typename base_container::const_iterator;

		/// Reverse iterator type
		using reverse_iterator       = typename base_container::reverse_iterator;

		/// Const reverse iterator type
		using const_reverse_iterator = typename base_container::const_reverse_iterator;

		/// Value type
		using value_type      = typename base_container::value_type;

		/// Reference type
		using reference       = typename base_container::reference;

		/// Const reference type
		using const_reference = typename base_container::const_reference;

		/** 
		 * @brief Default construtor
		 * 
		 * @note Resulting range will have no parent so it won't be useful for anything
		 */
		Range( )
		    : parent(nullptr),
		      constant_time_size(0)
		{
		}

		/** 
		 * @brief Constructs the range limited by 2 iterator
		 * 
		 * @param parent The underlying container
		 * @param begin_iterator The left inclusive bound of range
		 * @param end_iterator The right exclusive bound of range
		 * 
		 * @note The size variable is set by std::distance(begin_iterator, end_iterator)
		 */
		Range(base_container* parent, iterator begin_iterator, iterator end_iterator)
		    : parent(parent),
		      begin_iterator(begin_iterator),
		      end_iterator(end_iterator),
		      constant_time_size(std::distance(begin_iterator, end_iterator))
		{
		}

		/**
		 * @brief Constructs the range limited by 2 iterators
		 * 
		 * @param parent The underlying container
		 * @param begin_iterator The left inclusive bound of range
		 * @param end_iterator The right exclusive bound of range
		 * @param n The distance between begin_iterator and end_iterator
		 * 
		 * @warning Passing the value not equal to the distance between begin_iterator and end_iterator will
		 * create the range with inconsistent state that causes the undefined behavior
		 * 
		 */
		Range(base_container* parent, iterator begin_iterator, iterator end_iterator, size_t n)
		    : parent(parent),
		      begin_iterator(begin_iterator),
		      end_iterator(end_iterator),
		      constant_time_size(n)
		{
		}

		/** 
		 * @brief Move constructor
		 * 
		 * @param rhs The range to move
		 */
		Range(Range&& rhs)
		    : parent(rhs.parent),
		      begin_iterator(rhs.begin_iterator),
		      end_iterator(rhs.end_iterator),
		      constant_time_size(rhs.constant_time_size)
		{
			rhs.parent             = nullptr;
			rhs.begin_iterator     = iterator( );
			rhs.end_iterator       = iterator( );
			rhs.constant_time_size = 0;
		}

		/** 
		 * @brief Move assignment operator
		 * 
		 * @param rhs The range to transfer the state from
		 * 
		 * @return Reference to this
		 */
		Range& operator=(Range&& rhs)
		{
			parent             = rhs.parent;
			begin_iterator     = rhs.begin_iterator;
			end_iterator       = rhs.end_iterator;
			constant_time_size = rhs.constant_time_size;

			rhs.parent             = nullptr;
			rhs.begin_iterator     = iterator( );
			rhs.end_iterator       = iterator( );
			rhs.constant_time_size = 0;
			return *this;
		}

		/** 
		 * @brief Accesses the first element
		 * 
		 * @return The const reference to the first element
		 */
		const_reference front( ) const
		{
			return *begin_iterator;
		}

		/** 
		 * @brief Accesses the first element
		 * 
		 * @return The reference to the first element
		 */
		reference front( )
		{
			return *begin_iterator;
		}

		/** 
		 * @brief Accesses the last element
		 * 
		 * @return The const reference to the last element
		 */
		const_reference back( ) const
		{
			if(begin_iterator == end_iterator) return *end_iterator;
			return *std::prev(end_iterator);
		}

		/** 
		 * @brief Accesses the last element
		 * 
		 * @return The reference to the last element
		 */
		reference back( )
		{
			if(begin_iterator == end_iterator) return *end_iterator;
			return *std::prev(end_iterator);
		}

		/** 
		 * @brief Returns the iterator to the beginning
		 * 
		 * @return Iterator to the first element
		 */
		iterator begin( )
		{
			return begin_iterator;
		}

		/** 
		 * @brief Returns the const iterator to the beginning
		 * 
		 * @return Const iterator to the first element
		 */
		const_iterator begin( ) const
		{
			return begin_iterator;
		}

		/**
		 * @brief Returns the const iterator to the beginning
		 * 
		 * @return Const iterator to the first element
		 */
		const_iterator cbegin( ) const
		{
			return begin_iterator;
		}

		/** 
		 * @brief Returns an iterator to the end
		 * 
		 * @return Iterator to the element following the last element.
		 */
		iterator end( )
		{
			return end_iterator;
		}

		/** 
		 * @brief Returns an const iterator to the end
		 * 
		 * @return Const iterator to the element following the last element.
		 */
		const_iterator end( ) const
		{
			return end_iterator;
		}

		/** 
		 * @brief Returns an const iterator to the end
		 * 
		 * @return Const iterator to the element following the last element.
		 */
		const_iterator cend( ) const
		{
			return end_iterator;
		}

		/** 
		 * @brief Returns an reverse iterator to the beginning
		 * 
		 * @return Reverse iterator to the first element.
		 */
		reverse_iterator rbegin( )
		{
			return reverse_iterator(end_iterator);
		}

		/** 
		 * @brief Returns an const reverse iterator to the beginning
		 * 
		 * @return Const reverse iterator to the first element.
		 */
		const_reverse_iterator rbegin( ) const
		{
			return reverse_iterator(end_iterator);
		}

		/** 
		 * @brief Returns an const reverse iterator to the beginning
		 * 
		 * @return Const reverse iterator to the first element.
		 */
		const_reverse_iterator crbegin( ) const
		{
			return const_reverse_iterator(end_iterator);
		}

		/** 
		 * @brief Returns an reverse iterator to the end
		 * 
		 * @return Reverse iterator to the element following the last element.
		 */
		reverse_iterator rend( )
		{
			return reverse_iterator(begin_iterator);
		}

		/** 
		 * @brief Returns an const reverse iterator to the end
		 * 
		 * @return Const reverse iterator to the element following the last element.
		 */
		const_reverse_iterator rend( ) const
		{
			return reverse_iterator(begin_iterator);
		}

		/** 
		 * @brief Returns an const reverse iterator to the end
		 * 
		 * @return Const reverse iterator to the element following the last element.
		 */
		const_reverse_iterator crend( ) const
		{
			return const_reverse_iterator(begin_iterator);
		}

		/**
		 * @brief Checks whether the container is empty
		 * 
		 * @return true if the container is empty
		 */
		bool empty( ) const
		{
			return begin_iterator == end_iterator;
		}

		/** 
		 * @brief Returns the number of elements
		 * 
		 * @return The number of elements in the range
		 */
		size_t size( ) const
		{
			return constant_time_size;
		}

		/** 
		 * @brief Clears the contents
		 */
		void clear( )
		{
			parent->erase(begin_iterator, end_iterator, constant_time_size);
			constant_time_size = 0;
			begin_iterator     = end_iterator;
		}

		/** 
		 * @brief Inserts single element
		 * 
		 * @param pos Position to insert the element to
		 * @param value The element to insert
		 * 
		 * @return The iterator to the inserted element
		 */
		iterator insert(const_iterator pos, reference value)
		{
			return afterInsertionHook(pos, parent->insert(pos, value));
		}

		/** 
		 * @brief Inserts the range of elements
		 * 
		 * @param pos Position to insert the elements to
		 * @param first Iterator to the first element to insert
		 * @param last Iterator to the last element to insert
		 * 
		 * @return Iterator to the first inserted element
		 */
		template <typename InputIterator>
		iterator insert(const_iterator pos, InputIterator first, InputIterator last)
		{
			parent->insert(pos, first, last);
			return afterInsertionHook(pos, first);
		}

		/** 
		 * @brief Erases single element
		 * 
		 * @param pos Iterator to the element to erase
		 * 
		 * @return Iterator to the element following the erased one
		 */
		iterator erase(const_iterator pos)
		{
			beforeErasureHook(pos);
			return parent->erase(pos);
		}

		/** 
		 * @brief Erases a range of elements
		 * 
		 * @note The method calls std::distance(first, last) to calculate the count of erased elements
		 * 
		 * @param first Iterator to the first element to erase
		 * @param last Iterator to the element following the last element to erase
		 * 
		 * @return Iterator to the element following the last erased element
		 */
		iterator erase(const_iterator first, const_iterator last)
		{
			beforeErasureHook(first, last, std::distance(first, last));
			return parent->erase(first, last);
		}

		/**
		 * @brief Erases a range of elements assuming n to be the count of removed elements
		 * 
		 * @warning Passsing n value not equal to the count of elements in the range [first, last) causes undefined behavior
		 * 
		 * @param first Iterator to the first element to erase
		 * @param last Iterator to the element following the last element to erase
		 * @param n Count of removed elements
		 * 
		 * @return Iterator to the element following the last erased element
		 * 
		 */
		iterator erase(const_iterator first, const_iterator last, size_t n)
		{
			beforeErasureHook(first, last, n);
			return parent->erase(first, last, n);
		}

		/**
		 * @brief Adds an element to the end
		 * 
		 * @param value The value of the element to append
		 */
		void push_back(reference value)
		{
			afterInsertionHook(end_iterator, parent->insert(end_iterator, value));
		}

		/**
		 * @brief Adds and element to the beginning
		 * 
		 * @param value The value of the element to prepend
		 */
		void push_front(reference value)
		{
			afterInsertionHook(begin_iterator, parent->insert(begin_iterator, value));
		}

		/**
		 * @brief Removes the last element
		 */
		void pop_back( )
		{
			beforeErasureHook(std::prev(end_iterator));
			parent->erase(std::prev(end_iterator));
		}

		/** 
		 * @brief Removes the first element
		 */
		void pop_front( )
		{
			parent->erase(beforeErasureHook(begin_iterator));
		}

		/**
		 * @brief Moves all elements from another range
		 * 
		 * @param pos Element before which the content will be inserted
		 * @param rhs Another range to transfer the content from
		 */
		void splice(const_iterator pos, Range& rhs)
		{
			if(this == &rhs) return;
			beforeSpliceHook(pos, rhs);
			parent->splice(pos, *rhs.parent);
		}

		/**
		 * @brief Moves single element from another range
		 * 
		 * @param pos Element before which the content will be inserted
		 * @param rhs Another range to transfer the content from
		 * @param new_element The element to transfer from another range
		 */
		void splice(const_iterator pos, Range& rhs, const_iterator new_element)
		{
			if(new_element == pos) return;
			beforeSpliceHook(pos, rhs, new_element);
			parent->splice(pos, *rhs.parent, new_element);
		}

		/**
		 * @brief Transfer the range of elements from another range
		 * 
		 * @note This method calls std::distance(first, last) to calculate the count of transferred elements
		 * 
		 * @param pos Element before which the content will be inserted
		 * @param rhs Another range to transfer the content from
		 * @param first Iterator to the first element to transfer
		 * @param last Iterator to the element after the last element to transfer
		 */
		void splice(const_iterator pos, Range& rhs, const_iterator first, const_iterator last)
		{
			if(last == pos) return;
			beforeSpliceHook(pos, rhs, first, last);
			parent->splice(pos, *rhs.parent, first, last);
		}

		/**
		 * @brief Transfers the range of elements assuming n to be the count of removed elements
		 * 
		 * @warning Passsing n value not equal to the count of elements in the range [first, last) causes undefined behavior
		 * 
		 * @param pos Element before which the content will be inserted
		 * @param rhs Another range to transfer the content from
		 * @param first Iterator to the first element to transfer
		 * @param last Iterator to the element after the last element to transfer
		 * @param n The count of transferred elements
		 */
		void splice(const_iterator pos, Range& rhs, const_iterator first, const_iterator last, size_t n)
		{
			if(last == pos) return;
			beforeSpliceHook(pos, rhs, first, last, n);
			parent->splice(pos, *rhs.parent, first, last, n);
		}

		/**
		 * @brief Reverses the order of the elements
		 */
		void reverse( )
		{
			reverse(begin_iterator, end_iterator);
		}

		/**
		 * @brief Reverses the order of the elements in the range
		 * 
		 * @param first The iterator to the first element of range to reverse
		 * @param last The element pointing to the element after the last element of range to reverse
		 */
		void reverse(const_iterator first, const_iterator last)
		{
			if(first == last) return;
			while(std::prev(last) != first)
			{
				splice(first, *this, std::prev(last));
			}
		}

		/**
		 * @brief Removes all elements with specified value
		 * 
		 * @param value The value of elements to remove
		 */
		void remove(const_reference value)
		{
			remove_if([&value](const_reference v) { return v == value; }); // std::bind1st(std::equal_to<const_reference>(), value));
		}

		/**
		 * @brief Removes all elements satisfying specific criteria
		 * 
		 * @details Removes elements for which the passed predicate has returned true
		 * 
		 * @tparam Predicate The type of passed predicate
		 * @param pred The predicate to test the elements against
		 */
		template <typename Predicate>
		void remove_if(Predicate pred)
		{
			auto iter = begin_iterator;
			while(iter != end_iterator)
			{
				if(pred(*iter))
				{
					iter = erase(iter);
				}
				else
					++iter;
			}
		}

		/**
		 * @brief Adjusts the end of the range
		 * 
		 * @note The method calls std::distance(begin(), new_end_iterator) to calculate the new range size
		 * 
		 * @param new_end_iterator The iterator that must be considered as end()
		 */
		void setEnd(const_iterator new_end_iterator)
		{
			end_iterator       = removeIteratorConstness(new_end_iterator);
			constant_time_size = std::distance(begin_iterator, end_iterator);
		}

		/**
		 * @brief Adjusts the end of the range assuming new_size being the new range size
		 * 
		 * @warning Passing new_size not equal to the new range size causes the undefined behavior
		 * 
		 * @param new_end_iterator The iterator that must be considered as end()
		 * @param new_size The new size of range after adjusting the end
		 */
		void setEnd(const_iterator new_end_iterator, size_t new_size)
		{
			end_iterator       = removeIteratorConstness(new_end_iterator);
			constant_time_size = new_size;
		}

		/**
		 * @brief Erases and disposes elements in the range [first, last) assuming n to be the count of erased elements 
		 * 
		 * @warning Passing n not equal to the count of erased elements causes the undefined behavior
		 * 
		 * @tparam Disposer The type of elements disposer
		 * @param first Iterator to the first element to erase
		 * @param last Iterator to the element after the lase element to erase
		 * @param n Count of erased elements
		 * @param disposer Functor performing elements disposal
		 * 
		 */
		template <typename Disposer>
		void erase_and_dispose(const_iterator first, const_iterator last, size_t n, Disposer disposer)
		{
			beforeErasureHook(first, last, n);
			parent->erase_and_dispose(first, last, disposer);
		}


		/**
		 * @brief Erases and disposes elements in the range [first, last)
		 * 
		 * @warning This method calls std::distance(first, last) to calculate the count of erased elements
		 * 
		 * @tparam Disposer The type of elements disposer
		 * @param first Iterator to the first element to erase
		 * @param last Iterator to the element after the lase element to erase
		 * @param disposer Functor performing elements disposal
		 * 
		 */
		template <typename Disposer>
		void erase_and_dispose(const_iterator first, const_iterator last, Disposer disposer)
		{
			erase_and_dispose(first, last, std::distance(first, last), disposer);
		}

		/**
		 * @brief Removes and disposes all elements in the range
		 * 
		 * @tparam Disposer The type of elements disposer
		 * @param disposer Functor performing elements disposal
		 */
		template <typename Disposer>
		void clear_and_dispose(Disposer disposer)
		{
			erase_and_dispose(begin_iterator, end_iterator, size( ), disposer);
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
			auto next_iter                           = std::next(pos);
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
			if(begin_iterator == pos) begin_iterator = removeIteratorConstness(range.begin( ));
			if(&range != this)
			{
				range.constant_time_size = 0;
				range.begin_iterator     = range.end_iterator;
				constant_time_size += range.size( );
			}
		}

		void beforeSpliceHook(const_iterator pos, Range& range, const_iterator new_element)
		{
			bool rewrite_begin                      = false;
			if(begin_iterator == pos) rewrite_begin = true;
			if(new_element == range.begin_iterator) ++range.begin_iterator;
			if(&range != this)
			{
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
			bool rewrite_begin                                     = false;
			if(begin_iterator == pos) rewrite_begin                = true;
			if(first == range.begin_iterator) range.begin_iterator = removeIteratorConstness(last);
			if(&range != this)
			{
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