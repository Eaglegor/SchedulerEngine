#pragma once

#include "../DerivedTypeCaster.h"
#include <iterator>
#include <type_traits>

namespace Scheduler
{
	/**
	 * @ingroup collections
	 * 
	 * @brief This class wraps the container object and casts the elements of underlying container to the specified type on retrieval
	 * 
	 * @tparam CollectionType Type of the container
	 * @tparam AdoptedType Type to cast the elements to
	 * 
	 * @note AdoptedType must be CollectionType::value_type or the class derived from CollectionType::value_type
	 */
	template <typename CollectionType, typename AdoptedType>
	class RangeTypeAdapter
	{
	public:
		/**
		 * @brief Iterator class wrapping the underlying collection iterator and casting values to the specified class
		 * 
		 * @tparam T Type to cast the underlying iterator's value to
		 * @tparam BaseIterator Type of the underlying iterator
		 */ 
		template <typename T, typename BaseIterator>
		class Iterator : public std::iterator<std::bidirectional_iterator_tag, T>
		{
			friend class RangeTypeAdapter<CollectionType, AdoptedType>;

		public:
			using base_type = typename CollectionType::value_type;
			using new_type  = T;
			static_assert(std::is_base_of<base_type, new_type>::value, "Incompatible types");

			Iterator( )
			{
			}

			explicit Iterator(BaseIterator iter)
			    : iter(iter)
			{
			}

			template <typename Iter>
			Iterator(const Iter& rhs)
			    : iter(rhs.iter)
			{
			}

			template <typename Iter>
			Iterator& operator=(const Iter& rhs)
			{
				iter = rhs.iter;
				return *this;
			}

			Iterator& operator++( )
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

			Iterator& operator--( )
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

			bool operator==(const typename CollectionType::iterator& rhs) const
			{
				return iter == rhs;
			}

			bool operator!=(const Iterator& rhs) const
			{
				return !(*this == rhs);
			}

			T& operator*( ) const
			{
				return caster(*iter);
			}

			T* operator->( ) const
			{
				return caster(&*iter);
			}

			BaseIterator base( ) const
			{
				return iter;
			}

		private:
			DerivedTypeCaster<typename CollectionType::value_type, T> caster;
			BaseIterator iter;
		};

		using value_type             = AdoptedType;
		using size_type              = std::size_t;
		using difference_type        = std::ptrdiff_t;
		using reference              = value_type&;
		using const_reference        = const value_type&;
		using pointer                = value_type*;
		using const_pointer          = const value_type*;
		using iterator               = Iterator<AdoptedType, typename CollectionType::iterator>;
		using const_iterator         = Iterator<const AdoptedType, typename CollectionType::const_iterator>;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		/**
		 * @brief Constructs RangeTypeAdapter without underlying collection
		 * 
		 * @deprecated In fact this constructor produces the unusable range. It's worth considering getting rid of it
		 * 
		 */ 
		RangeTypeAdapter( )
		    : backend_collection(nullptr)
		{
		}

		/**
		 * @brief Constructs RangeTypeAdapter wrapping collection
		 * 
		 * @param collection Pointer to the collection to wrap
		 */ 
		RangeTypeAdapter(CollectionType* collection)
		    : backend_collection(collection)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs RangeTypeAdapater to copy the state from
		 */
		RangeTypeAdapter(const RangeTypeAdapter& rhs)
		    : backend_collection(rhs.backend_collection)
		{
		}

		/**
		 * @brief Assignment operator
		 * 
		 * @param rhs RangeTypeAdapter to copy the state from
		 */
		RangeTypeAdapter& operator=(const RangeTypeAdapter& rhs)
		{
			backend_collection = rhs.backend_collection;
			return *this;
		}

		/**
		 * @brief Returns the first element
		 */
		reference front( )
		{
			return static_cast<reference>(backend_collection->front( ));
		}

		/**
		 * @brief Returns the first element
		 */
		const_reference front( ) const
		{
			return static_cast<const_reference>(backend_collection->front( ));
		}

		/**
		 * @brief Returns the last element
		 */
		reference back( )
		{
			return static_cast<reference>(backend_collection->back( ));
		}

		/**
		 * @brief Returns the last element
		 */
		const_reference back( ) const
		{
			return static_cast<const_reference>(backend_collection->back( ));
		}

		/**
		 * @brief Returns iterator pointing to the beginning
		 * 
		 * @return Iterator pointing to the first element
		 */
		iterator begin( )
		{
			return iterator(backend_collection->begin( ));
		}

		/**
		 * @brief Returns iterator pointing to the beginning
		 * 
		 * @return Iterator pointing to the first element
		 */
		const_iterator begin( ) const
		{
			return const_iterator(backend_collection->cbegin( ));
		}

		/**
		 * @brief Returns iterator pointing to the beginning
		 * 
		 * @return Iterator pointing to the first element
		 */
		const_iterator cbegin( ) const
		{
			return const_iterator(backend_collection->cbegin( ));
		}

		/**
		 * @brief Returns iterator pointing to the end
		 * 
		 * @return Iterator pointing to the element after the last element
		 */
		iterator end( )
		{
			return iterator(backend_collection->end( ));
		}

		/**
		 * @brief Returns iterator pointing to the end
		 * 
		 * @return Iterator pointing to the element after the last element
		 */
		const_iterator end( ) const
		{
			return const_iterator(backend_collection->cend( ));
		}

		/**
		 * @brief Returns iterator pointing to the end
		 * 
		 * @return Iterator pointing to the element after the last element
		 */
		const_iterator cend( ) const
		{
			return const_iterator(backend_collection->cend( ));
		}

		/**
		 * @brief Returns reverse iterator pointing to the beginning
		 * 
		 * @return Reverse iterator pointing to the first element
		 */
		reverse_iterator rbegin( )
		{
			return reverse_iterator(end( ));
		}

		/**
		 * @brief Returns reverse iterator pointing to the beginning
		 * 
		 * @return Reverse iterator pointing to the first element
		 */
		const_reverse_iterator rbegin( ) const
		{
			return const_reverse_iterator(end( ));
		}

		/**
		 * @brief Returns reverse iterator pointing to the beginning
		 * 
		 * @return Reverse iterator pointing to the first element
		 */
		const_reverse_iterator crbegin( ) const
		{
			return const_reverse_iterator(end( ));
		}

		/**
		 * @brief Returns reverse iterator pointing to the end
		 * 
		 * @return Reverse iterator pointing to the element after the last element
		 */
		reverse_iterator rend( )
		{
			return reverse_iterator(begin( ));
		}

		/**
		 * @brief Returns reverse iterator pointing to the end
		 * 
		 * @return Reverse iterator pointing to the element after the last element
		 */
		const_reverse_iterator rend( ) const
		{
			return const_reverse_iterator(begin( ));
		}

		/**
		 * @brief Returns reverse iterator pointing to the end
		 * 
		 * @return Reverse iterator pointing to the element after the last element
		 */
		const_reverse_iterator crend( ) const
		{
			return const_reverse_iterator(begin( ));
		}

		/**
		 * @brief Checks if the underlying container is empty
		 * 
		 * @return True if the underlying container is empty
		 */ 
		bool empty( ) const
		{
			return backend_collection->empty( );
		}

		/**
		 * @brief Returns the size of the underlying container
		 * 
		 * @return The size of the underlying container
		 */
		size_type size( ) const
		{
			return backend_collection->size( );
		}

		/**
		 * @brief Returns the max size of the underlying container
		 * 
		 * @return Max size of the underlying container
		 */
		size_type max_size( ) const
		{
			return backend_collection->max_size( );
		}

		/**
		 * @brief Removes all elements from the underlying container
		 */
		void clear( )
		{
			backend_collection->clear( );
		}

		/**
		 * @brief Inserts the element into the underlying container
		 * 
		 * @param pos Iterator to the element before which the element will be inserted
		 * @param value Value of the new element
		 * 
		 * @return Iterator to the inserted element
		 */
		iterator insert(const_iterator pos, reference value)
		{
			return iterator(backend_collection->insert(pos.iter, value));
		}

		/**
		 * @brief Inserts the range of elements into the underlying container
		 * 
		 * @param pos Iterator to the element before which the element will be inserted
		 * @param first Iterator to the first element to be inserted
		 * @param last Iterator to the element after the last element to be inserted
		 * 
		 * @return Iterator to the first if the inserted elements
		 */
		iterator insert(const_iterator pos, const_iterator first, const_iterator last)
		{
			return iterator(backend_collection->insert(pos.iter, first.iter, last.iter));
		}

		/**
		 * @brief Erases the element from the underlying container
		 * 
		 * @param pos Iterator to the element to be erased
		 * 
		 * @return Iterator to the element after the erased one
		 */
		iterator erase(const_iterator pos)
		{
			return iterator(backend_collection->erase(pos.iter));
		}

		/**
		 * @brief Erases the range of elements into the underlying container
		 * 
		 * @param first Iterator to the first element to be erased
		 * @param last Iterator to the element after the last element to be erased
		 * 
		 * @return Iterator to the element after the last erased one
		 */
		iterator erase(const_iterator first, const_iterator last)
		{
			return iterator(backend_collection->erase(first.iter, last.iter));
		}

		/**
		 * @brief Inserts the element into the end of underlying container
		 * 
		 * @param value The value of element to insert
		 */
		void push_back(reference value)
		{
			backend_collection->push_back(value);
		}

		/**
		 * @brief Removes the last element from the underlying container
		 */
		void pop_back( )
		{
			backend_collection->pop_back( );
		}

		/**
		 * @brief Inserts the element into the beginning of underlying container
		 * 
		 * @param value The value of element to insert
		 */
		void push_front(reference value)
		{
			backend_collection->push_front(value);
		}

		/**
		 * @brief Removes the first element from the underlying container
		 */
		void pop_front( )
		{
			backend_collection->pop_front( );
		}

		/**
		 * @brief Transfer the single element from another RangeTypeAdapter
		 * 
		 * @param pos Element before which the content will be inserted
		 * @param other Another RangeTypeAdapter to transfer the content from
		 * @param element Iterator to the element to transfer
		 */
		void splice(const_iterator pos, RangeTypeAdapter<CollectionType, AdoptedType>& other, const_iterator element)
		{
			backend_collection->splice(pos.iter, *other.backend_collection, element.iter);
		}

		/**
		 * @brief Transfer the range of elements from another RangeTypeAdapter
		 * 
		 * @param pos Element before which the content will be inserted
		 * @param other Another RangeTypeAdapter to transfer the content from
		 * @param first Iterator to the first element to be transferred
		 * @param last Iterator to the element after the last element to be transferred
		 */
		void splice(const_iterator pos, RangeTypeAdapter<CollectionType, AdoptedType>& other, const_iterator first, const_iterator last)
		{
			backend_collection->splice(pos.iter, *other.backend_collection, first.iter, last.iter);
		}

		/**
		 * @brief Transfer the range of elements from another RangeTypeAdapter assuming n is as the count of transferred elements
		 * 
		 * @param pos Element before which the content will be inserted
		 * @param other Another RangeTypeAdapter to transfer the content from
		 * @param first Iterator to the first element to be transferred
		 * @param last Iterator to the element after the last element to be transferred
		 * @param n Count of transferred elements
		 */
		void splice(const_iterator pos, RangeTypeAdapter<CollectionType, AdoptedType>& other, const_iterator first, const_iterator last, size_t n)
		{
			backend_collection->splice(pos.iter, *other.backend_collection, first.iter, last.iter, n);
		}

		/**
		 * @brief Removes all elements with specified value
		 * 
		 * @param value The value of elements to remove
		 */
		void remove(reference value)
		{
			backend_collection->remove(value);
		}

		/**
		 * @brief Removes all elements satisfying specific criteria
		 * 
		 * @details Removes elements for which the passed predicate has returned true
		 * 
		 * @tparam UnaryPredicate The type of passed predicate
		 * @param p The predicate to test the elements against
		 */
		template <typename UnaryPredicate>
		void remove_if(UnaryPredicate p)
		{
			iterator current = begin( );
			while(current != end( ))
			{
				iterator next = std::next(current);
				if(p(*current)) erase(current);
				current = next;
			}
		}

		/**
		 * @brief Reverses the order of the elements
		 */
		void reverse( )
		{
			backend_collection->reverse( );
		}

		/**
		 * @brief Reverses the order of the elements in the range
		 * 
		 * @param first The iterator to the first element of range to reverse
		 * @param last The element pointing to the element after the last element of range to reverse
		 */
		void reverse(const_iterator first, const_iterator last)
		{
			backend_collection->reverse(first.iter, last.iter);
		}

	private:
		CollectionType* backend_collection;
	};
}