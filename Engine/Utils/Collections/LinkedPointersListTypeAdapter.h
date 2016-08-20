#pragma once

#include <iterator>
#include <type_traits>

namespace Scheduler
{
	template<typename CollectionType, typename AdoptedType>
	class LinkedPointersListTypeAdapter
	{
	public:
		
		class Iterator : public std::iterator<std::bidirectional_iterator_tag, AdoptedType>
		{
		friend class LinkedPointersListTypeAdapter<CollectionType, AdoptedType>;
			
		public:
			using base_type = typename std::remove_pointer<typename CollectionType::value_type>::type;
			using new_type = typename std::remove_pointer<AdoptedType>::type;
			static_assert(std::is_base_of<base_type, new_type>::value, "Incompatible types");
			
			Iterator(){}
			
			explicit Iterator(const typename CollectionType::iterator &iter):
			iter(iter)
			{
			}
			
			Iterator(const Iterator& rhs):
			iter(rhs.iter)
			{
			}
			
			Iterator& operator=(const Iterator& rhs)
			{
				iter = rhs.iter;
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
			
			const AdoptedType& operator*() const
			{
				return static_cast<const AdoptedType&>(*iter);
			}
			
			AdoptedType operator*()
			{
				return static_cast<AdoptedType>(*iter);
			}
			
			const AdoptedType* operator->() const
			{
				return static_cast<const AdoptedType*>(&*iter);
			}
			
			AdoptedType* operator->()
			{
				return static_cast<AdoptedType*>(&*iter);
			}
			
		private:
			typename CollectionType::iterator iter;
		};

		using value_type = AdoptedType;
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
		
		LinkedPointersListTypeAdapter(CollectionType& collection):
		backend_collection(collection)
		{}
		
		reference front()
		{
			return static_cast<reference>(backend_collection.front());
		}
		
		const_reference front() const
		{
			return static_cast<const_reference>(backend_collection.front());
		}
		
		reference back()
		{
			return static_cast<reference>(backend_collection.back());
		}
		
		const_reference back() const
		{
			return static_cast<const_reference>(backend_collection.back());
		}
		
		iterator begin() const
		{
			return iterator(backend_collection.begin());
		}
		
		const_iterator cbegin() const
		{
			return const_iterator(backend_collection.cbegin());
		}
		
		iterator end() const
		{
			return iterator(backend_collection.end());
		}
		
		const_iterator cend() const
		{
			return const_iterator(backend_collection.cend());
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
			return backend_collection.empty();
		}
		
		size_type size() const
		{
			return backend_collection.size();
		}
		
		size_type max_size() const
		{
			return backend_collection.max_size();
		}
		
		void clear()
		{
			backend_collection.clear();
		}
		
		iterator insert(iterator pos, value_type value)
		{
			return iterator(backend_collection.insert(pos.iter, value));
		}
		
		iterator insert(iterator pos, iterator first, iterator last)
		{
			return iterator(backend_collection.insert(pos.iter, first.iter, last.iter));
		}
		
		iterator erase(iterator pos)
		{
			return iterator(backend_collection.erase(pos.iter));
		}
		
		iterator erase(iterator first, iterator last)
		{
			return iterator(backend_collection.erase(first.iter, last.iter));
		}
		
		void push_back(value_type value)
		{
			backend_collection.push_back(value);
		}
		
		void pop_back()
		{
			backend_collection.pop_back();
		}

		void push_front(const_reference value)
		{
			backend_collection.push_fron(value);
		}
		
		void pop_front()
		{
			backend_collection.pop_front();
		}
		
		void splice(iterator pos, LinkedPointersListTypeAdapter<CollectionType, AdoptedType>& other, iterator first, iterator last)
		{
			backend_collection.splice(pos.iter, other.backend_collection, first.iter, last.iter);
		}
		
		void remove(value_type value)
		{
			backend_collection.remove(value);
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
			backend_collection.reverse();
		}
		
		void reverse(iterator first, iterator last)
		{
			backend_collection.reverse(first.iter, last.iter);
		}
		
	private:
		CollectionType& backend_collection;
		
	};
	
	
}