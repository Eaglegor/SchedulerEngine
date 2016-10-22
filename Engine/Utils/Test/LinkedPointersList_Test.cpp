#include <catch.hpp>

#include <boost/intrusive/list.hpp>
#include <Engine/Utils/Collections/Range.h>
#include <type_traits>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>

class Node : public boost::intrusive::list_base_hook<>
{
	public:	
		Node():_value(0){}
		
		std::size_t value() const
		{
			return _value;
		}
		
		void setValue(size_t value)
		{
			this->_value = value;
		}
		
		Node& operator=(int value)
		{
			this->_value = value;
			return *this;
		}
		
		bool operator==(const Node& rhs) const
		{
			return this == &rhs;
		}
		
		bool operator==(const Node* rhs) const
		{
			return this == rhs;
		}
		
	private:
		std::size_t _value;
};


template<typename T>
using LinkedPointersList = boost::intrusive::list< typename std::remove_pointer<T>::type >;

template<typename T, typename base = LinkedPointersList<T>>
using LinkedPointersSublist = Scheduler::Range<base>;

namespace std
{
	using namespace Scheduler;
	
	template<>
	class front_insert_iterator<LinkedPointersList<Node*>> : public iterator<Node, output_iterator_tag>
	{
	public:
		front_insert_iterator(LinkedPointersList<Node*>& cont):cont(cont){}
		front_insert_iterator& operator=(typename LinkedPointersList<Node*>::reference value){cont.push_front(value);}
		
		front_insert_iterator& operator*(){return *this;}
		void operator++(){}
		
	private:
		LinkedPointersList<Node*>& cont;
	};
	
	template<>
	class front_insert_iterator<LinkedPointersSublist<Node*>> : public iterator<Node, output_iterator_tag>
	{
	public:
		front_insert_iterator(LinkedPointersSublist<Node*>& cont):cont(cont){}
		front_insert_iterator& operator=(typename LinkedPointersSublist<Node*>::reference value){cont.push_front(value);}
		
		front_insert_iterator& operator*(){return *this;}
		void operator++(){}
		
	private:
		LinkedPointersSublist<Node*>& cont;
	};
	
	template<>
	class front_insert_iterator<LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>>> : public iterator<Node, output_iterator_tag>
	{
	public:
		front_insert_iterator(LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>>& cont):cont(cont){}
		front_insert_iterator& operator=(typename LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>>::reference value){cont.push_front(value);}
		
		front_insert_iterator& operator*(){return *this;}
		void operator++(){}
		
	private:
		LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>>& cont;
	};
	
	template<>
	class back_insert_iterator<LinkedPointersList<Node*>> : public iterator<Node, output_iterator_tag>
	{
	public:
		back_insert_iterator(LinkedPointersList<Node*>& cont):cont(cont){}
		back_insert_iterator& operator=(typename LinkedPointersList<Node*>::reference value){cont.push_back(value);}
		
		back_insert_iterator& operator*(){return *this;}
		void operator++(){}
		
	private:
		LinkedPointersList<Node*>& cont;
	};
	
	template<>
	class back_insert_iterator<LinkedPointersSublist<Node*>> : public iterator<Node, output_iterator_tag>
	{
	public:
		back_insert_iterator(LinkedPointersSublist<Node*>& cont):cont(cont){}
		back_insert_iterator& operator=(typename LinkedPointersSublist<Node*>::reference value){cont.push_back(value);}
		
		back_insert_iterator& operator*(){return *this;}
		void operator++(){}
		
	private:
		LinkedPointersSublist<Node*>& cont;
	};
	
	template<>
	class back_insert_iterator<LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>>> : public iterator<Node, output_iterator_tag>
	{
	public:
		back_insert_iterator(LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>>& cont):cont(cont){}
		back_insert_iterator& operator=(typename LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>>::reference value){cont.push_back(value);}
		
		back_insert_iterator& operator*(){return *this;}
		void operator++(){}
		
	private:
		LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>>& cont;
	};
}

TEST_CASE("LinkedPointersList", "[unit][functonal]")
{
	using namespace Scheduler;
	
	std::vector<Node> nodes(10);
	
	for(size_t i = 0; i < 10; ++i)
	{
		nodes[i].setValue(i);
	}

	
	LinkedPointersList<Node*> nodes_list;
	SECTION("Default state")
	{
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
	}
	
	SECTION("Push + iterate + clear")
	{
		SECTION("Push back")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(nodes_list));
		}
		SECTION("Push front")
		{
			std::copy(nodes.rbegin(), nodes.rend(), std::front_inserter(nodes_list));
		}

		REQUIRE(!nodes_list.empty());
		REQUIRE(nodes_list.size() == 10);

		LinkedPointersList<Node*>::iterator current = nodes_list.begin();
		for(int i = 0; i < nodes.size(); ++i)
		{
			REQUIRE((*current).value() == i);
			++current;
		}
	
		LinkedPointersList<Node*>::reverse_iterator rcurrent = nodes_list.rbegin();
		for(int i = 0; i < nodes.size(); ++i)
		{
			REQUIRE((*rcurrent).value() == nodes.size() - 1 - i);
			++rcurrent;
		}
		
		nodes_list.clear();
		
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
	}
	
	SECTION("Pop")
	{
		std::copy(nodes.begin(), nodes.end(), std::back_inserter(nodes_list));
		
		REQUIRE(!nodes_list.empty());
		REQUIRE(nodes_list.size() == 10);
		
		SECTION("Single pop")
		{
			SECTION("Pop back")
			{
				nodes_list.pop_back();
				REQUIRE(nodes_list.size() == 9);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE((*nodes_list.begin()).value() == 0);
				REQUIRE((*nodes_list.rbegin()).value() == 8);
			}
			SECTION("Pop front")
			{
				nodes_list.pop_front();
				REQUIRE(nodes_list.size() == 9);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE((*nodes_list.begin()).value() == 1);
				REQUIRE((*nodes_list.rbegin()).value() == 9);
			}
		}
		
		SECTION("Clear pop")
		{
			SECTION("Pop back")
			{
				for(int i = 0; i < nodes.size(); ++i)
				{
					nodes_list.pop_back();
				}
			}
			
			SECTION("Pop front")
			{
				for(int i = 0; i < nodes.size(); ++i)
				{
					nodes_list.pop_front();
				}
			}
			
			REQUIRE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 0);
		}
	}
	
	SECTION("Front + back")
	{
		std::copy(nodes.begin(), nodes.end(), std::back_inserter(nodes_list));
		
		REQUIRE(nodes_list.front().value() == 0);
		REQUIRE(nodes_list.back().value() == 9);
	}
	
	SECTION("Reverse")
	{
		std::copy(nodes.begin(), nodes.end(), std::back_inserter(nodes_list));
		
		SECTION("Full")
		{
			nodes_list.reverse();
			
			LinkedPointersList<Node*>::iterator current = nodes_list.begin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				REQUIRE((*current).value() == nodes.size() - 1 - i);
				++current;
			}
		}
	}
	
	SECTION("Insert + erase")
	{
		nodes_list.insert(nodes_list.begin(), nodes[nodes.size()-1]);
		nodes_list.insert(nodes_list.end(), nodes[0]);
		LinkedPointersList<Node*>::iterator iter = nodes_list.insert(std::next(nodes_list.begin()), nodes[nodes.size()-2]);
		nodes_list.insert(iter, nodes[1]);
		nodes_list.insert(iter, nodes[2]);
		nodes_list.insert(iter, nodes[3]);
		
		REQUIRE_FALSE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 6);
		REQUIRE(*std::next(nodes_list.begin(), 0) == nodes[nodes.size() - 1]);
		REQUIRE(*std::next(nodes_list.begin(), 1) == nodes[1]);
		REQUIRE(*std::next(nodes_list.begin(), 2) == nodes[2]);
		REQUIRE(*std::next(nodes_list.begin(), 3) == nodes[3]);
		REQUIRE(*std::next(nodes_list.begin(), 4) == nodes[nodes.size() - 2]);
		REQUIRE(*std::next(nodes_list.begin(), 5) == nodes[0]);
		
		nodes_list.erase(std::next(nodes_list.begin(), 3));
		nodes_list.erase(std::next(nodes_list.begin(), 0));
		nodes_list.erase(std::next(nodes_list.begin(), 3));
		
		REQUIRE_FALSE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 3);
		REQUIRE(*std::next(nodes_list.begin(), 0) == nodes[1]);
		REQUIRE(*std::next(nodes_list.begin(), 1) == nodes[2]);
		REQUIRE(*std::next(nodes_list.begin(), 2) == nodes[nodes.size() - 2]);
		
		nodes_list.erase(std::next(nodes_list.begin(), 0));
		nodes_list.erase(std::next(nodes_list.begin(), 0));
		nodes_list.erase(std::next(nodes_list.begin(), 0));
		
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
	}
	
	SECTION("Remove")
	{
		std::copy(nodes.begin(), nodes.end(), std::back_inserter(nodes_list));
		
		SECTION("Value")
		{
			nodes_list.remove(nodes[4]);
			
			REQUIRE(nodes_list.size() == 9);
			
			int current_index = 0;
			auto iter = nodes_list.begin();
			while(current_index < nodes.size())
			{
				if(current_index == 4) ++current_index;
				REQUIRE(*iter == nodes[current_index]);
				++iter;
				++current_index;
			}
		}
		
		SECTION("Predicate")
		{
			nodes_list.remove_if([&](const Node& val){return val.value() >= 5;});
			
			REQUIRE(nodes_list.size() == 5);
			auto current = nodes_list.begin();
			for(int i = 0; i < 5; ++i)
			{
				REQUIRE(*current == nodes[i]);
				++current;
			}
		}
	}
	
	SECTION("Splice")
	{
		LinkedPointersList<Node*> nodes_list2;
		
		std::copy(nodes.begin(), std::next(nodes.begin(), 5), std::back_inserter(nodes_list));
		std::copy(nodes.begin() + 5, nodes.end(), std::back_inserter(nodes_list2));
		
		REQUIRE(nodes_list.size() == 5);
		REQUIRE(nodes_list2.size() == 5);
		
		{
			auto current = nodes_list.begin();
			for(int i = 0; i < 5; ++i)
			{
				REQUIRE(*current == nodes[i]);
				++current;
			}
		}
		
		{
			auto current = nodes_list2.begin();
			for(int i = 5; i < 10; ++i)
			{
				REQUIRE(*current == nodes[i]);
				++current;
			}
		}
		
		SECTION("Full other splice")
		{
			nodes_list.splice(std::next(nodes_list.begin()), nodes_list2, nodes_list2.begin(), nodes_list2.end());
			
			REQUIRE(nodes_list.size() == 10);
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list2.size() == 0);
			REQUIRE(nodes_list2.empty());
			
			auto current = nodes_list.begin();
			
			REQUIRE(*(current++) == nodes[0]);
			
			for(int i = 5; i < 10; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
		}
		
		SECTION("Single other splice")
		{
			nodes_list.splice(std::next(nodes_list.begin()), nodes_list2, std::next(nodes_list2.begin()), std::next(nodes_list2.begin(), 2));
			
			REQUIRE(nodes_list.size() == 6);
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list2.size() == 4);
			REQUIRE_FALSE(nodes_list2.empty());
			
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[0]);
			REQUIRE(*(current++) == nodes[6]);
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
			
			current = nodes_list2.begin();
			REQUIRE(*(current++) == nodes[5]);
			for(int i = 7; i < 10; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
		}
		
		SECTION("Partial other splice")
		{
			nodes_list.splice(std::next(nodes_list.begin()), nodes_list2, std::next(nodes_list2.begin()), std::next(nodes_list2.begin(), 3));
			
			REQUIRE(nodes_list.size() == 7);
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list2.size() == 3);
			REQUIRE_FALSE(nodes_list2.empty());
			
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[0]);
			REQUIRE(*(current++) == nodes[6]);
			REQUIRE(*(current++) == nodes[7]);
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
			
			current = nodes_list2.begin();
			REQUIRE(*(current++) == nodes[5]);
			for(int i = 8; i < 10; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
		}
		
		SECTION("Other splice -> begin")
		{
			nodes_list.splice(nodes_list.begin(), nodes_list2, std::next(nodes_list2.begin()), std::next(nodes_list2.begin(), 3));
			
			REQUIRE(nodes_list.size() == 7);
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list2.size() == 3);
			REQUIRE_FALSE(nodes_list2.empty());
			
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[6]);
			REQUIRE(*(current++) == nodes[7]);
			REQUIRE(*(current++) == nodes[0]);
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
			
			current = nodes_list2.begin();
			REQUIRE(*(current++) == nodes[5]);
			for(int i = 8; i < 10; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
		}
		
		SECTION("Other splice -> end")
		{
			nodes_list.splice(nodes_list.end(), nodes_list2, std::next(nodes_list2.begin()), std::next(nodes_list2.begin(), 3));
			
			REQUIRE(nodes_list.size() == 7);
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list2.size() == 3);
			REQUIRE_FALSE(nodes_list2.empty());
			
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[0]);
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
			REQUIRE(*(current++) == nodes[6]);
			REQUIRE(*(current++) == nodes[7]);
			
			current = nodes_list2.begin();
			REQUIRE(*(current++) == nodes[5]);
			for(int i = 8; i < 10; ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
		}
		
		SECTION("Single self splice")
		{
			nodes_list.splice(std::next(nodes_list.begin()), nodes_list, std::next(nodes_list.begin(), 3), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[0]);
			REQUIRE(*(current++) == nodes[3]);
			REQUIRE(*(current++) == nodes[1]);
			REQUIRE(*(current++) == nodes[2]);
			REQUIRE(*(current++) == nodes[4]);
		}
		
		/* Boost intrusive doesn't correctly process this case
		SECTION("Same position self splice")
		{
			nodes_list.splice(std::next(nodes_list.begin(), 4), nodes_list, std::next(nodes_list.begin(), 3), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[0]);
			REQUIRE(*(current++) == nodes[1]);
			REQUIRE(*(current++) == nodes[2]);
			REQUIRE(*(current++) == nodes[3]);
			REQUIRE(*(current++) == nodes[4]);
		}
		*/
		
		SECTION("Previous position self splice")
		{
			nodes_list.splice(std::next(nodes_list.begin(), 2), nodes_list, std::next(nodes_list.begin(), 3), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[0]);
			REQUIRE(*(current++) == nodes[1]);
			REQUIRE(*(current++) == nodes[3]);
			REQUIRE(*(current++) == nodes[2]);
			REQUIRE(*(current++) == nodes[4]);
		}
		
		
		SECTION("Multi self splice")
		{
			nodes_list.splice(std::next(nodes_list.begin()), nodes_list, std::next(nodes_list.begin(), 2), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[0]);
			REQUIRE(*(current++) == nodes[2]);
			REQUIRE(*(current++) == nodes[3]);
			REQUIRE(*(current++) == nodes[1]);
			REQUIRE(*(current++) == nodes[4]);
		}
		
		SECTION("Self splice -> begin")
		{
			nodes_list.splice(nodes_list.begin(), nodes_list, std::next(nodes_list.begin(), 2), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[2]);
			REQUIRE(*(current++) == nodes[3]);
			REQUIRE(*(current++) == nodes[0]);
			REQUIRE(*(current++) == nodes[1]);
			REQUIRE(*(current++) == nodes[4]);
		}
		
		SECTION("Self splice -> end")
		{
			nodes_list.splice(nodes_list.end(), nodes_list, std::next(nodes_list.begin(), 2), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == nodes[0]);
			REQUIRE(*(current++) == nodes[1]);
			REQUIRE(*(current++) == nodes[4]);
			REQUIRE(*(current++) == nodes[2]);
			REQUIRE(*(current++) == nodes[3]);
		}
		
	}
}


TEST_CASE("LinkedPointersSublist")
{
	using namespace Scheduler;
	
	std::vector<Node> nodes(10);
	std::iota(nodes.begin(), nodes.end(), 0);

	LinkedPointersList<Node*> nodes_list;
	
	SECTION("EmptyList")
	{
		LinkedPointersSublist<Node*> sublist(&nodes_list, nodes_list.begin(), nodes_list.end());
		
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
		REQUIRE(sublist.empty());
		REQUIRE(sublist.size() == 0);
		
		SECTION("Push + iterate + clear")
		{
			SECTION("Push back")
			{
				std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			}
			SECTION("Push front")
			{
				std::copy(nodes.rbegin(), nodes.rend(), std::front_inserter(sublist));
			}

			REQUIRE(!nodes_list.empty());
			REQUIRE(nodes_list.size() == 10);
			REQUIRE(!sublist.empty());
			REQUIRE(sublist.size() == 10);
			
			auto current = sublist.begin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
		
			auto rcurrent = sublist.rbegin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				const auto& n = *(rcurrent++);
				const auto& n1 = nodes[nodes.size() - 1 - i];
				REQUIRE(n == n1);
			}
			
			sublist.clear();
			
			REQUIRE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 0);
			REQUIRE(sublist.empty());
			REQUIRE(sublist.size() == 0);
		}
		
		SECTION("Pop")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			REQUIRE(!sublist.empty());
			REQUIRE(sublist.size() == 10);
			REQUIRE(!nodes_list.empty());
			REQUIRE(nodes_list.size() == 10);
			
			SECTION("Single pop")
			{
				SECTION("Pop back")
				{
					sublist.pop_back();
					REQUIRE(nodes_list.size() == 9);
					REQUIRE_FALSE(nodes_list.empty());
					REQUIRE((*nodes_list.begin()).value() == 0);
					REQUIRE((*nodes_list.rbegin()).value() == 8);
					
					REQUIRE(sublist.size() == 9);
					REQUIRE_FALSE(sublist.empty());
					REQUIRE((*sublist.begin()).value() == 0);
					REQUIRE((*sublist.rbegin()).value() == 8);
				}
				SECTION("Pop front")
				{
					sublist.pop_front();
					REQUIRE(nodes_list.size() == 9);
					REQUIRE_FALSE(nodes_list.empty());
					REQUIRE((*nodes_list.begin()).value() == 1);
					REQUIRE((*nodes_list.rbegin()).value() == 9);
					
					REQUIRE(sublist.size() == 9);
					REQUIRE_FALSE(sublist.empty());
					REQUIRE((*sublist.begin()).value() == 1);
					REQUIRE((*sublist.rbegin()).value() == 9);
				}
			}
			
			SECTION("Clear pop")
			{
				SECTION("Pop back")
				{
					for(int i = 0; i < nodes.size(); ++i)
					{
						sublist.pop_back();
					}
				}
				
				SECTION("Pop front")
				{
					for(int i = 0; i < nodes.size(); ++i)
					{
						sublist.pop_front();
					}
				}
				
				REQUIRE(nodes_list.empty());
				REQUIRE(nodes_list.size() == 0);
				
				REQUIRE(sublist.empty());
				REQUIRE(sublist.size() == 0);
			}
			
		}
		
		SECTION("Front + back")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			REQUIRE(nodes_list.front().value() == 0);
			REQUIRE(nodes_list.back().value() == 9);
			
			REQUIRE(sublist.front().value() == 0);
			REQUIRE(sublist.back().value() == 9);
		}
		
		SECTION("Reverse")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
		
			SECTION("Full")
			{
				sublist.reverse();
				
				auto current = sublist.begin();
				for(int i = 0; i < nodes.size(); ++i)
				{
					const auto& n = *(current++);
					const auto& n1 = nodes[nodes.size() - 1 - i];
					REQUIRE(n == n1);
				}
			}
			SECTION("Partial")
			{
				sublist.reverse(std::next(sublist.begin()), std::next(sublist.begin(), 4));

				REQUIRE(*std::next(sublist.begin(), 0) == nodes[0]);
				REQUIRE(*std::next(sublist.begin(), 1) == nodes[3]);
				REQUIRE(*std::next(sublist.begin(), 2) == nodes[2]);
				REQUIRE(*std::next(sublist.begin(), 3) == nodes[1]);
				REQUIRE(*std::next(sublist.begin(), 4) == nodes[4]);
				
				LinkedPointersList<Node*>::iterator current = std::next(sublist.begin(), 4);
				for(int i = 4; i < nodes.size(); ++i)
				{
					REQUIRE((*current).value() == i);
					++current;
				}
			}
		}
		
		SECTION("Insert + erase")
		{
			sublist.insert(sublist.begin(), nodes[nodes.size()-1]);
			sublist.insert(sublist.end(), nodes[0]);
			auto iter = sublist.insert(std::next(sublist.begin()), nodes[nodes.size()-2]);
			sublist.insert(iter, nodes[1]);
			sublist.insert(iter, nodes[2]);
			sublist.insert(iter, nodes[3]);
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 6);
			REQUIRE(*std::next(nodes_list.begin(), 0) == nodes[nodes.size() - 1]);
			REQUIRE(*std::next(nodes_list.begin(), 1) == nodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == nodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == nodes[3]);
			REQUIRE(*std::next(nodes_list.begin(), 4) == nodes[nodes.size() - 2]);
			REQUIRE(*std::next(nodes_list.begin(), 5) == nodes[0]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 6);
			REQUIRE(*std::next(sublist.begin(), 0) == nodes[nodes.size() - 1]);
			REQUIRE(*std::next(sublist.begin(), 1) == nodes[1]);
			REQUIRE(*std::next(sublist.begin(), 2) == nodes[2]);
			REQUIRE(*std::next(sublist.begin(), 3) == nodes[3]);
			REQUIRE(*std::next(sublist.begin(), 4) == nodes[nodes.size() - 2]);
			REQUIRE(*std::next(sublist.begin(), 5) == nodes[0]);
			
			sublist.erase(std::next(sublist.begin(), 3));
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 3));
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 3);
			REQUIRE(*std::next(nodes_list.begin(), 0) == nodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 1) == nodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == nodes[nodes.size() - 2]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 3);
			REQUIRE(*std::next(sublist.begin(), 0) == nodes[1]);
			REQUIRE(*std::next(sublist.begin(), 1) == nodes[2]);
			REQUIRE(*std::next(sublist.begin(), 2) == nodes[nodes.size() - 2]);
			
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 0));
			
			REQUIRE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 0);
			
			REQUIRE(sublist.empty());
			REQUIRE(sublist.size() == 0);
		}
		
		SECTION("Remove")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			SECTION("Value")
			{
				sublist.remove(nodes[4]);
				
				REQUIRE(nodes_list.size() == 9);
				REQUIRE(sublist.size() == 9);
				
				int current_index = 0;
				auto iter = sublist.begin();
				while(current_index < nodes.size())
				{
					if(current_index == 4) ++current_index;
					REQUIRE(*iter == nodes[current_index]);
					++iter;
					++current_index;
				}
			}
			
			SECTION("Predicate")
			{
				sublist.remove_if([&](Node& val){return val.value() >= 5;});
				
				REQUIRE(nodes_list.size() == 5);
				REQUIRE(sublist.size() == 5);
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*current == nodes[i]);
					++current;
				}
			}
		}
		
		SECTION("Splice")
		{
			LinkedPointersList<Node*> nodes_list2;
			LinkedPointersSublist<Node*> sublist2(&nodes_list2, nodes_list2.begin(), nodes_list2.end());
			
			std::copy(nodes.begin(), std::next(nodes.begin(), 5), std::back_inserter(sublist));
			std::copy(nodes.begin() + 5, nodes.end(), std::back_inserter(sublist2));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE(nodes_list2.size() == 5);
			
			REQUIRE(sublist.size() == 5);
			REQUIRE(sublist2.size() == 5);
			
			{
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			{
				auto current = sublist2.begin();
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Full other splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist2, sublist2.begin(), sublist2.end());
				
				REQUIRE(nodes_list.size() == 10);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list2.size() == 0);
				REQUIRE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 10);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 0);
				REQUIRE(sublist2.empty());
				
				auto current = sublist.begin();
				
				REQUIRE(*(current++) == nodes[0]);
				
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Single other splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 2));
				
				REQUIRE(nodes_list.size() == 6);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list2.size() == 4);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 6);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 4);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[6]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 7; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Partial other splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 3));
				
				REQUIRE(nodes_list.size() == 7);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list2.size() == 3);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 7);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 3);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[6]);
				REQUIRE(*(current++) == nodes[7]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				
				current = nodes_list2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Other splice -> begin")
			{
				sublist.splice(sublist.begin(), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 3));
				
				REQUIRE(nodes_list.size() == 7);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list2.size() == 3);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 7);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 3);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[6]);
				REQUIRE(*(current++) == nodes[7]);
				REQUIRE(*(current++) == nodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Other splice -> end")
			{
				sublist.splice(sublist.end(), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 3));
				
				REQUIRE(nodes_list.size() == 7);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list2.size() == 3);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 7);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 3);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				REQUIRE(*(current++) == nodes[6]);
				REQUIRE(*(current++) == nodes[7]);
				
				current = sublist2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Single self splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist, std::next(sublist.begin(), 3), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 5);
				REQUIRE_FALSE(nodes_list.empty());
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Same position self splice")
			{
				sublist.splice(std::next(sublist.begin(), 4), sublist, std::next(sublist.begin(), 3), std::next(sublist.begin(), 4));
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[4]);
			}

			SECTION("Previous position self splice")
			{
				sublist.splice(std::next(sublist.begin(), 2), sublist, std::next(sublist.begin(), 3), std::next(sublist.begin(), 4));
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Multi self splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 5);
				REQUIRE_FALSE(nodes_list.empty());
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Self splice -> begin")
			{
				sublist.splice(sublist.begin(), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 5);
				REQUIRE_FALSE(nodes_list.empty());
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Self splice -> end")
			{
				sublist.splice(sublist.end(), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 5);
				REQUIRE_FALSE(nodes_list.empty());
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[4]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
			}
		}
	}
	
	SECTION("Middle sublist")
	{
		Node start_node;
		start_node.setValue(100);
		Node end_node;
		end_node.setValue(200);
		
		nodes_list.push_back(end_node);
		nodes_list.push_front(start_node);
		
		LinkedPointersSublist<Node*> sublist(&nodes_list, std::next(nodes_list.begin()), std::prev(nodes_list.end()));
		
		REQUIRE_FALSE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 2);
		REQUIRE(nodes_list.front() == &start_node);
		REQUIRE(nodes_list.back() == &end_node);
		REQUIRE(sublist.empty());
		REQUIRE(sublist.size() == 0);
		
		SECTION("Push + iterate + clear")
		{
			SECTION("Push back")
			{
				std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			}
			SECTION("Push front")
			{
				std::copy(nodes.rbegin(), nodes.rend(), std::front_inserter(sublist));
			}

			REQUIRE(!nodes_list.empty());
			REQUIRE(nodes_list.size() == 12);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			REQUIRE(!sublist.empty());
			REQUIRE(sublist.size() == 10);
			
			auto current = sublist.begin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
		
			auto rcurrent = sublist.rbegin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				const auto& n = *(rcurrent++);
				const auto& n1 = nodes[nodes.size() - 1 - i];
				REQUIRE(n == n1);
			}
			
			sublist.clear();
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 2);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			REQUIRE(sublist.empty());
			REQUIRE(sublist.size() == 0);
		}
		
		SECTION("Pop")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			REQUIRE(!sublist.empty());
			REQUIRE(sublist.size() == 10);
			REQUIRE(!nodes_list.empty());
			REQUIRE(nodes_list.size() == 12);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			
			SECTION("Single pop")
			{
				SECTION("Pop back")
				{
					sublist.pop_back();
					REQUIRE(nodes_list.size() == 11);
					REQUIRE_FALSE(nodes_list.empty());
					REQUIRE(nodes_list.front() == &start_node);
					REQUIRE(nodes_list.back() == &end_node);
					
					REQUIRE(sublist.size() == 9);
					REQUIRE_FALSE(sublist.empty());
					REQUIRE((*sublist.begin()).value() == 0);
					REQUIRE((*sublist.rbegin()).value() == 8);
				}
				SECTION("Pop front")
				{
					sublist.pop_front();
					REQUIRE(nodes_list.size() == 11);
					REQUIRE_FALSE(nodes_list.empty());
					REQUIRE(nodes_list.front() == &start_node);
					REQUIRE(nodes_list.back() == &end_node);
					
					REQUIRE(sublist.size() == 9);
					REQUIRE_FALSE(sublist.empty());
					REQUIRE((*sublist.begin()).value() == 1);
					REQUIRE((*sublist.rbegin()).value() == 9);
				}
			}
			
			SECTION("Clear pop")
			{
				SECTION("Pop back")
				{
					for(int i = 0; i < nodes.size(); ++i)
					{
						sublist.pop_back();
					}
				}
				
				SECTION("Pop front")
				{
					for(int i = 0; i < nodes.size(); ++i)
					{
						sublist.pop_front();
					}
				}
				
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.size() == 2);
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				
				REQUIRE(sublist.empty());
				REQUIRE(sublist.size() == 0);
			}
			
		}
		
		SECTION("Front + back")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			
			REQUIRE(sublist.front().value() == 0);
			REQUIRE(sublist.back().value() == 9);
		}
		
		SECTION("Reverse")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			sublist.reverse();
			
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			
			auto current = sublist.begin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				const auto& n = *(current++);
				const auto& n1 = nodes[nodes.size() - 1 - i];
				REQUIRE(n == n1);
			}
		}
		
		SECTION("Insert + erase")
		{
			sublist.insert(sublist.begin(), nodes[nodes.size()-1]);
			sublist.insert(sublist.end(), nodes[0]);
			auto iter = sublist.insert(std::next(sublist.begin()), nodes[nodes.size()-2]);
			sublist.insert(iter, nodes[1]);
			sublist.insert(iter, nodes[2]);
			sublist.insert(iter, nodes[3]);
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 8);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			REQUIRE(*std::next(nodes_list.begin(), 1) == nodes[nodes.size() - 1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == nodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == nodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 4) == nodes[3]);
			REQUIRE(*std::next(nodes_list.begin(), 5) == nodes[nodes.size() - 2]);
			REQUIRE(*std::next(nodes_list.begin(), 6) == nodes[0]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 6);
			REQUIRE(*std::next(sublist.begin(), 0) == nodes[nodes.size() - 1]);
			REQUIRE(*std::next(sublist.begin(), 1) == nodes[1]);
			REQUIRE(*std::next(sublist.begin(), 2) == nodes[2]);
			REQUIRE(*std::next(sublist.begin(), 3) == nodes[3]);
			REQUIRE(*std::next(sublist.begin(), 4) == nodes[nodes.size() - 2]);
			REQUIRE(*std::next(sublist.begin(), 5) == nodes[0]);
			
			sublist.erase(std::next(sublist.begin(), 3));
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 3));
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			REQUIRE(nodes_list.size() == 5);
			REQUIRE(*std::next(nodes_list.begin(), 1) == nodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == nodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == nodes[nodes.size() - 2]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 3);
			REQUIRE(*std::next(sublist.begin(), 0) == nodes[1]);
			REQUIRE(*std::next(sublist.begin(), 1) == nodes[2]);
			REQUIRE(*std::next(sublist.begin(), 2) == nodes[nodes.size() - 2]);
			
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 0));
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 2);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			
			REQUIRE(sublist.empty());
			REQUIRE(sublist.size() == 0);
		}
		
		SECTION("Remove")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			SECTION("Value")
			{
				sublist.remove(nodes[4]);
				
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				REQUIRE(nodes_list.size() == 11);
				REQUIRE(sublist.size() == 9);
				
				int current_index = 0;
				auto iter = sublist.begin();
				while(current_index < nodes.size())
				{
					if(current_index == 4) ++current_index;
					REQUIRE(*iter == nodes[current_index]);
					++iter;
					++current_index;
				}
			}
			
			SECTION("Predicate")
			{
				sublist.remove_if([&](Node& val){return val.value() >= 5;});
				
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				REQUIRE(nodes_list.size() == 7);
				REQUIRE(sublist.size() == 5);
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*current == nodes[i]);
					++current;
				}
			}
		}
		
		SECTION("Splice")
		{
			LinkedPointersList<Node*> nodes_list2;
			LinkedPointersSublist<Node*> sublist2(&nodes_list2, nodes_list2.begin(), nodes_list2.end());
			
			std::copy(nodes.begin(), std::next(nodes.begin(), 5), std::back_inserter(sublist));
			std::copy(nodes.begin() + 5, nodes.end(), std::back_inserter(sublist2));
			
			REQUIRE(nodes_list.size() == 7);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			REQUIRE(nodes_list2.size() == 5);
			
			REQUIRE(sublist.size() == 5);
			REQUIRE(sublist2.size() == 5);
			
			{
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			{
				auto current = sublist2.begin();
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Full other splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist2, sublist2.begin(), sublist2.end());
				
				REQUIRE(nodes_list.size() == 12);
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list2.size() == 0);
				REQUIRE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 10);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 0);
				REQUIRE(sublist2.empty());
				
				auto current = sublist.begin();
				
				REQUIRE(*(current++) == nodes[0]);
				
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Single other splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 2));
				
				REQUIRE(nodes_list.size() == 8);
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list2.size() == 4);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 6);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 4);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[6]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 7; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Partial other splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 3));
				
				REQUIRE(nodes_list.size() == 9);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				REQUIRE(nodes_list2.size() == 3);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 7);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 3);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[6]);
				REQUIRE(*(current++) == nodes[7]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				
				current = nodes_list2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Other splice -> begin")
			{
				sublist.splice(sublist.begin(), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 3));
				
				REQUIRE(nodes_list.size() == 9);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				REQUIRE(nodes_list2.size() == 3);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 7);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 3);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[6]);
				REQUIRE(*(current++) == nodes[7]);
				REQUIRE(*(current++) == nodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Other splice -> end")
			{
				sublist.splice(sublist.end(), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 3));
				
				REQUIRE(nodes_list.size() == 9);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				REQUIRE(nodes_list2.size() == 3);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 7);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 3);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				REQUIRE(*(current++) == nodes[6]);
				REQUIRE(*(current++) == nodes[7]);
				
				current = sublist2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Single self splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist, std::next(sublist.begin(), 3), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 7);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Multi self splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 7);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Self splice -> begin")
			{
				sublist.splice(sublist.begin(), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 7);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Self splice -> end")
			{
				sublist.splice(sublist.end(), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 7);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[4]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
			}
		}
		nodes_list.clear();
	}

	SECTION("Tail sublist")
	{
		Node start_node;
		start_node.setValue(100);
		
		nodes_list.push_front(start_node);
		
		LinkedPointersSublist<Node*> sublist(&nodes_list, std::next(nodes_list.begin()), nodes_list.end());
		
		REQUIRE_FALSE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 1);
		REQUIRE(nodes_list.front() == &start_node);
		REQUIRE(sublist.empty());
		REQUIRE(sublist.size() == 0);
		
		SECTION("Push + iterate + clear")
		{
			SECTION("Push back")
			{
				std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			}
			SECTION("Push front")
			{
				std::copy(nodes.rbegin(), nodes.rend(), std::front_inserter(sublist));
			}

			REQUIRE(!nodes_list.empty());
			REQUIRE(nodes_list.size() == 11);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(!sublist.empty());
			REQUIRE(sublist.size() == 10);
			
			auto current = sublist.begin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				REQUIRE(*(current++) == nodes[i]);
			}
		
			auto rcurrent = sublist.rbegin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				const auto& n = *(rcurrent++);
				const auto& n1 = nodes[nodes.size() - 1 - i];
				REQUIRE(n == n1);
			}
			
			sublist.clear();
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 1);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(sublist.empty());
			REQUIRE(sublist.size() == 0);
		}
		
		SECTION("Pop")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			REQUIRE(!sublist.empty());
			REQUIRE(sublist.size() == 10);
			REQUIRE(!nodes_list.empty());
			REQUIRE(nodes_list.size() == 11);
			REQUIRE(nodes_list.front() == &start_node);
			
			SECTION("Single pop")
			{
				SECTION("Pop back")
				{
					sublist.pop_back();
					REQUIRE(nodes_list.size() == 10);
					REQUIRE_FALSE(nodes_list.empty());
					REQUIRE(nodes_list.front() == &start_node);
					
					REQUIRE(sublist.size() == 9);
					REQUIRE_FALSE(sublist.empty());
					REQUIRE((*sublist.begin()).value() == 0);
					REQUIRE((*sublist.rbegin()).value() == 8);
				}
				SECTION("Pop front")
				{
					sublist.pop_front();
					REQUIRE(nodes_list.size() == 10);
					REQUIRE_FALSE(nodes_list.empty());
					REQUIRE(nodes_list.front() == &start_node);
					
					REQUIRE(sublist.size() == 9);
					REQUIRE_FALSE(sublist.empty());
					REQUIRE((*sublist.begin()).value() == 1);
					REQUIRE((*sublist.rbegin()).value() == 9);
				}
			}
			
			SECTION("Clear pop")
			{
				SECTION("Pop back")
				{
					for(int i = 0; i < nodes.size(); ++i)
					{
						sublist.pop_back();
					}
				}
				
				SECTION("Pop front")
				{
					for(int i = 0; i < nodes.size(); ++i)
					{
						sublist.pop_front();
					}
				}
				
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.size() == 1);
				REQUIRE(nodes_list.front() == &start_node);
				
				REQUIRE(sublist.empty());
				REQUIRE(sublist.size() == 0);
			}
			
		}
		
		SECTION("Front + back")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			REQUIRE(nodes_list.front() == &start_node);
			
			REQUIRE(sublist.front().value() == 0);
			REQUIRE(sublist.back().value() == 9);
		}
		
		SECTION("Reverse")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			sublist.reverse();
			
			REQUIRE(nodes_list.front() == &start_node);
			
			auto current = sublist.begin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				const auto& n = *(current++);
				const auto& n1 = nodes[nodes.size() - 1 - i];
				REQUIRE(n == n1);
			}
		}
		
		SECTION("Insert + erase")
		{
			sublist.insert(sublist.begin(), nodes[nodes.size()-1]);
			sublist.insert(sublist.end(), nodes[0]);
			auto iter = sublist.insert(std::next(sublist.begin()), nodes[nodes.size()-2]);
			sublist.insert(iter, nodes[1]);
			sublist.insert(iter, nodes[2]);
			sublist.insert(iter, nodes[3]);
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 7);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(*std::next(nodes_list.begin(), 1) == nodes[nodes.size() - 1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == nodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == nodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 4) == nodes[3]);
			REQUIRE(*std::next(nodes_list.begin(), 5) == nodes[nodes.size() - 2]);
			REQUIRE(*std::next(nodes_list.begin(), 6) == nodes[0]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 6);
			REQUIRE(*std::next(sublist.begin(), 0) == nodes[nodes.size() - 1]);
			REQUIRE(*std::next(sublist.begin(), 1) == nodes[1]);
			REQUIRE(*std::next(sublist.begin(), 2) == nodes[2]);
			REQUIRE(*std::next(sublist.begin(), 3) == nodes[3]);
			REQUIRE(*std::next(sublist.begin(), 4) == nodes[nodes.size() - 2]);
			REQUIRE(*std::next(sublist.begin(), 5) == nodes[0]);
			
			sublist.erase(std::next(sublist.begin(), 3));
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 3));
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.size() == 4);
			REQUIRE(*std::next(nodes_list.begin(), 1) == nodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == nodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == nodes[nodes.size() - 2]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 3);
			REQUIRE(*std::next(sublist.begin(), 0) == nodes[1]);
			REQUIRE(*std::next(sublist.begin(), 1) == nodes[2]);
			REQUIRE(*std::next(sublist.begin(), 2) == nodes[nodes.size() - 2]);
			
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 0));
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 1);
			REQUIRE(nodes_list.front() == &start_node);
			
			REQUIRE(sublist.empty());
			REQUIRE(sublist.size() == 0);
		}
		
		SECTION("Remove")
		{
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(sublist));
			
			SECTION("Value")
			{
				sublist.remove(nodes[4]);
				
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.size() == 10);
				REQUIRE(sublist.size() == 9);
				
				int current_index = 0;
				auto iter = sublist.begin();
				while(current_index < nodes.size())
				{
					if(current_index == 4) ++current_index;
					REQUIRE(*iter == nodes[current_index]);
					++iter;
					++current_index;
				}
			}
			
			SECTION("Predicate")
			{
				sublist.remove_if([&](Node& val){return val.value() >= 5;});
				
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.size() == 6);
				REQUIRE(sublist.size() == 5);
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*current == nodes[i]);
					++current;
				}
			}
		}
		
		SECTION("Splice")
		{
			LinkedPointersList<Node*> nodes_list2;
			LinkedPointersSublist<Node*> sublist2(&nodes_list2, nodes_list2.begin(), nodes_list2.end());
			
			std::copy(nodes.begin(), std::next(nodes.begin(), 5), std::back_inserter(sublist));
			std::copy(nodes.begin() + 5, nodes.end(), std::back_inserter(sublist2));
			
			REQUIRE(nodes_list.size() == 6);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list2.size() == 5);
			
			REQUIRE(sublist.size() == 5);
			REQUIRE(sublist2.size() == 5);
			
			{
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			{
				auto current = sublist2.begin();
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Full other splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist2, sublist2.begin(), sublist2.end());
				
				REQUIRE(nodes_list.size() == 11);
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list2.size() == 0);
				REQUIRE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 10);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 0);
				REQUIRE(sublist2.empty());
				
				auto current = sublist.begin();
				
				REQUIRE(*(current++) == nodes[0]);
				
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Single other splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 2));
				
				REQUIRE(nodes_list.size() == 7);
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list2.size() == 4);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 6);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 4);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[6]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 7; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Partial other splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 3));
				
				REQUIRE(nodes_list.size() == 8);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list2.size() == 3);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 7);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 3);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[6]);
				REQUIRE(*(current++) == nodes[7]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				
				current = nodes_list2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Other splice -> begin")
			{
				sublist.splice(sublist.begin(), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 3));
				
				REQUIRE(nodes_list.size() == 8);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list2.size() == 3);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 7);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 3);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[6]);
				REQUIRE(*(current++) == nodes[7]);
				REQUIRE(*(current++) == nodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Other splice -> end")
			{
				sublist.splice(sublist.end(), sublist2, std::next(sublist2.begin()), std::next(sublist2.begin(), 3));
				
				REQUIRE(nodes_list.size() == 8);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list2.size() == 3);
				REQUIRE_FALSE(nodes_list2.empty());
				
				REQUIRE(sublist.size() == 7);
				REQUIRE_FALSE(sublist.empty());
				REQUIRE(sublist2.size() == 3);
				REQUIRE_FALSE(sublist2.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
				REQUIRE(*(current++) == nodes[6]);
				REQUIRE(*(current++) == nodes[7]);
				
				current = sublist2.begin();
				REQUIRE(*(current++) == nodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == nodes[i]);
				}
			}
			
			SECTION("Single self splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist, std::next(sublist.begin(), 3), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 6);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Multi self splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 6);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Self splice -> begin")
			{
				sublist.splice(sublist.begin(), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 6);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[4]);
			}
			
			SECTION("Self splice -> end")
			{
				sublist.splice(sublist.end(), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 6);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE(nodes_list.front() == &start_node);
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == nodes[0]);
				REQUIRE(*(current++) == nodes[1]);
				REQUIRE(*(current++) == nodes[4]);
				REQUIRE(*(current++) == nodes[2]);
				REQUIRE(*(current++) == nodes[3]);
			}
		}
		
		nodes_list.clear();
	}
}


TEST_CASE("Nested sublists")
{
	using namespace Scheduler;
	
	std::vector<Node> nodes(10);
	
	for(size_t i = 0; i < 10; ++i)
	{
		nodes[i].setValue(i);
	}
	
	LinkedPointersList<Node*> nodes_list;
	
	nodes_list.push_front(nodes[0]);
	nodes_list.push_back(nodes[8]);
	nodes_list.push_back(nodes[9]);
	
	LinkedPointersSublist<Node*> sublist1(&nodes_list, std::next(nodes_list.begin()), std::prev(nodes_list.end()));
	
	sublist1.push_front(nodes[7]);	
	sublist1.push_front(nodes[6]);
	sublist1.push_front(nodes[1]);
	
	LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>> sublist2(&sublist1, std::next(sublist1.begin()), std::next(sublist1.begin(), 2));
	
	sublist2.push_front(nodes[5]);
	sublist2.push_front(nodes[4]);
	sublist2.push_front(nodes[3]);
	sublist2.push_front(nodes[2]);
	
	REQUIRE(nodes_list.size() == 10);
	REQUIRE(sublist1.size() == 8);
	REQUIRE(sublist2.size() == 5);
	
	LinkedPointersList<Node*>::iterator current = nodes_list.begin();
	for(int i = 0; i < nodes.size(); ++i)
	{
		REQUIRE((*current).value() == i);
		++current;
	}
	
	sublist2.clear();
	REQUIRE(nodes_list.size() == 5);
	REQUIRE(sublist1.size() == 3);
	
	sublist1.clear();
	REQUIRE(nodes_list.size() == 2);
	
}