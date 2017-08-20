#pragma once

#include "../DerivedTypeCaster.h"
#include <iterator>
#include <type_traits>

namespace Scheduler
{
	template <typename CollectionType, typename AdoptedType>
	class RangeTypeAdapter
	{
	public:
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

		RangeTypeAdapter( )
		    : backend_collection(nullptr)
		{
		}

		RangeTypeAdapter(CollectionType* collection)
		    : backend_collection(collection)
		{
		}

		RangeTypeAdapter(const RangeTypeAdapter& rhs)
		    : backend_collection(rhs.backend_collection)
		{
		}

		RangeTypeAdapter& operator=(const RangeTypeAdapter& rhs)
		{
			backend_collection = rhs.backend_collection;
			return *this;
		}

		reference front( )
		{
			return static_cast<reference>(backend_collection->front( ));
		}

		const_reference front( ) const
		{
			return static_cast<const_reference>(backend_collection->front( ));
		}

		reference back( )
		{
			return static_cast<reference>(backend_collection->back( ));
		}

		const_reference back( ) const
		{
			return static_cast<const_reference>(backend_collection->back( ));
		}

		iterator begin( )
		{
			return iterator(backend_collection->begin( ));
		}

		const_iterator begin( ) const
		{
			return const_iterator(backend_collection->cbegin( ));
		}

		const_iterator cbegin( ) const
		{
			return const_iterator(backend_collection->cbegin( ));
		}

		iterator end( )
		{
			return iterator(backend_collection->end( ));
		}

		const_iterator end( ) const
		{
			return const_iterator(backend_collection->cend( ));
		}

		const_iterator cend( ) const
		{
			return const_iterator(backend_collection->cend( ));
		}

		reverse_iterator rbegin( )
		{
			return reverse_iterator(end( ));
		}

		const_reverse_iterator rbegin( ) const
		{
			return const_reverse_iterator(end( ));
		}

		const_reverse_iterator crbegin( ) const
		{
			return const_reverse_iterator(end( ));
		}

		reverse_iterator rend( )
		{
			return reverse_iterator(begin( ));
		}

		const_reverse_iterator rend( ) const
		{
			return const_reverse_iterator(begin( ));
		}

		const_reverse_iterator crend( ) const
		{
			return const_reverse_iterator(begin( ));
		}

		bool empty( ) const
		{
			return backend_collection->empty( );
		}

		size_type size( ) const
		{
			return backend_collection->size( );
		}

		size_type max_size( ) const
		{
			return backend_collection->max_size( );
		}

		void clear( )
		{
			backend_collection->clear( );
		}

		iterator insert(const_iterator pos, reference value)
		{
			return iterator(backend_collection->insert(pos.iter, value));
		}

		iterator insert(const_iterator pos, const_iterator first, const_iterator last)
		{
			return iterator(backend_collection->insert(pos.iter, first.iter, last.iter));
		}

		iterator erase(const_iterator pos)
		{
			return iterator(backend_collection->erase(pos.iter));
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			return iterator(backend_collection->erase(first.iter, last.iter));
		}

		void push_back(reference value)
		{
			backend_collection->push_back(value);
		}

		void pop_back( )
		{
			backend_collection->pop_back( );
		}

		void push_front(reference value)
		{
			backend_collection->push_front(value);
		}

		void pop_front( )
		{
			backend_collection->pop_front( );
		}

		void splice(const_iterator pos, RangeTypeAdapter<CollectionType, AdoptedType>& other, const_iterator element)
		{
			backend_collection->splice(pos.iter, *other.backend_collection, element.iter);
		}

		void splice(const_iterator pos, RangeTypeAdapter<CollectionType, AdoptedType>& other, const_iterator first, const_iterator last)
		{
			backend_collection->splice(pos.iter, *other.backend_collection, first.iter, last.iter);
		}

		void splice(const_iterator pos, RangeTypeAdapter<CollectionType, AdoptedType>& other, const_iterator first, const_iterator last, size_t n)
		{
			backend_collection->splice(pos.iter, *other.backend_collection, first.iter, last.iter, n);
		}

		void remove(reference value)
		{
			backend_collection->remove(value);
		}

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

		void reverse( )
		{
			backend_collection->reverse( );
		}

		void reverse(const_iterator first, const_iterator last)
		{
			backend_collection->reverse(first.iter, last.iter);
		}

	private:
		CollectionType* backend_collection;
	};
}