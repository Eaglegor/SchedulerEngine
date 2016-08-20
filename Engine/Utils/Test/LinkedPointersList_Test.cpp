#include <catch.hpp>

#include <Engine/Utils/Collections/LinkedPointersList.h>
#include <Engine/Utils/Collections/LinkedPointersSublist.h>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <iostream>

class Node
{
	public:	
		Node():_prev(nullptr),_next(nullptr),_value(0){}
		
		Node* next()
		{
			return _next;
		}
		
		Node* prev()
		{
			return _prev;
		}
		
		void setNext(Node* node)
		{
			_next = node;
		}
		
		void setPrev(Node* node)
		{
			_prev = node;
		}
		
		size_t value()
		{
			return _value;
		}
		
		void setValue(size_t value)
		{
			this->_value = value;
		}
		
	private:
		size_t _value;
		Node* _next;
		Node* _prev;
};

TEST_CASE("LinkedPointersList", "[unit][functonal]")
{
	using namespace Scheduler;
	
	std::vector<Node> nodes(10);
	std::vector<Node*> pnodes(10);
	
	for(size_t i = 0; i < 10; ++i)
	{
		nodes[i].setValue(i);
		pnodes[i] = &nodes[i];
	}
	
	
	LinkedPointersList<Node*> nodes_list;
	SECTION("Default state")
	{
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
		REQUIRE(nodes_list.max_size() == std::numeric_limits<size_t>::max());
	}
	
	SECTION("Push + iterate + clear")
	{
		SECTION("Push back")
		{
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(nodes_list));
		}
		SECTION("Push front")
		{
			std::copy(pnodes.rbegin(), pnodes.rend(), std::front_inserter(nodes_list));
		}

		REQUIRE(!nodes_list.empty());
		REQUIRE(nodes_list.size() == 10);
		
		LinkedPointersList<Node*>::iterator current = nodes_list.begin();
		for(int i = 0; i < nodes.size(); ++i)
		{
			REQUIRE((*current)->value() == i);
			++current;
		}
	
		LinkedPointersList<Node*>::reverse_iterator rcurrent = nodes_list.rbegin();
		for(int i = 0; i < nodes.size(); ++i)
		{
			REQUIRE((*rcurrent)->value() == nodes.size() - 1 - i);
			++rcurrent;
		}
		
		nodes_list.clear();
		
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
	}
	
	SECTION("Pop")
	{
		std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(nodes_list));
		
		REQUIRE(!nodes_list.empty());
		REQUIRE(nodes_list.size() == 10);
		
		SECTION("Single pop")
		{
			SECTION("Pop back")
			{
				nodes_list.pop_back();
				REQUIRE(nodes_list.size() == 9);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE((*nodes_list.begin())->value() == 0);
				REQUIRE((*nodes_list.rbegin())->value() == 8);
			}
			SECTION("Pop front")
			{
				nodes_list.pop_front();
				REQUIRE(nodes_list.size() == 9);
				REQUIRE_FALSE(nodes_list.empty());
				REQUIRE((*nodes_list.begin())->value() == 1);
				REQUIRE((*nodes_list.rbegin())->value() == 9);
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
		std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(nodes_list));
		
		REQUIRE(nodes_list.front()->value() == 0);
		REQUIRE(nodes_list.back()->value() == 9);
	}
	
	SECTION("Reverse")
	{
		std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(nodes_list));
		
		SECTION("Full")
		{
			nodes_list.reverse();
			
			LinkedPointersList<Node*>::iterator current = nodes_list.begin();
			for(int i = 0; i < nodes.size(); ++i)
			{
				REQUIRE((*current)->value() == nodes.size() - 1 - i);
				++current;
			}
		}
		
		SECTION("Partial")
		{
			nodes_list.reverse(std::next(nodes_list.begin()), std::next(nodes_list.begin(), 4));

			REQUIRE(*std::next(nodes_list.begin(), 0) == pnodes[0]);
			REQUIRE(*std::next(nodes_list.begin(), 1) == pnodes[3]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == pnodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == pnodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 4) == pnodes[4]);
			
			LinkedPointersList<Node*>::iterator current = std::next(nodes_list.begin(), 4);
			for(int i = 4; i < nodes.size(); ++i)
			{
				REQUIRE((*current)->value() == i);
				++current;
			}
		}
	}
	
	SECTION("Insert + erase")
	{
		nodes_list.insert(nodes_list.begin(), pnodes[pnodes.size()-1]);
		nodes_list.insert(nodes_list.end(), pnodes[0]);
		LinkedPointersList<Node*>::iterator iter = nodes_list.insert(std::next(nodes_list.begin()), pnodes[pnodes.size()-2]);
		nodes_list.insert(iter, pnodes[1]);
		nodes_list.insert(iter, pnodes[2]);
		nodes_list.insert(iter, pnodes[3]);
		
		REQUIRE_FALSE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 6);
		REQUIRE(*std::next(nodes_list.begin(), 0) == pnodes[pnodes.size() - 1]);
		REQUIRE(*std::next(nodes_list.begin(), 1) == pnodes[1]);
		REQUIRE(*std::next(nodes_list.begin(), 2) == pnodes[2]);
		REQUIRE(*std::next(nodes_list.begin(), 3) == pnodes[3]);
		REQUIRE(*std::next(nodes_list.begin(), 4) == pnodes[pnodes.size() - 2]);
		REQUIRE(*std::next(nodes_list.begin(), 5) == pnodes[0]);
		
		nodes_list.erase(std::next(nodes_list.begin(), 3));
		nodes_list.erase(std::next(nodes_list.begin(), 0));
		nodes_list.erase(std::next(nodes_list.begin(), 3));
		
		REQUIRE_FALSE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 3);
		REQUIRE(*std::next(nodes_list.begin(), 0) == pnodes[1]);
		REQUIRE(*std::next(nodes_list.begin(), 1) == pnodes[2]);
		REQUIRE(*std::next(nodes_list.begin(), 2) == pnodes[pnodes.size() - 2]);
		
		nodes_list.erase(std::next(nodes_list.begin(), 0));
		nodes_list.erase(std::next(nodes_list.begin(), 0));
		nodes_list.erase(std::next(nodes_list.begin(), 0));
		
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
	}
	
	SECTION("Remove")
	{
		std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(nodes_list));
		
		SECTION("Value")
		{
			nodes_list.remove(pnodes[4]);
			
			REQUIRE(nodes_list.size() == 9);
			
			int current_index = 0;
			auto iter = nodes_list.begin();
			while(current_index < pnodes.size())
			{
				if(current_index == 4) ++current_index;
				REQUIRE(*iter == pnodes[current_index]);
				++iter;
				++current_index;
			}
		}
		
		SECTION("Predicate")
		{
			nodes_list.remove_if([&](Node* val){return val->value() >= 5;});
			
			REQUIRE(nodes_list.size() == 5);
			auto current = nodes_list.begin();
			for(int i = 0; i < 5; ++i)
			{
				REQUIRE(*current == pnodes[i]);
				++current;
			}
		}
	}
	
	SECTION("Splice")
	{
		LinkedPointersList<Node*> nodes_list2;
		
		std::copy(pnodes.begin(), std::next(pnodes.begin(), 5), std::back_inserter(nodes_list));
		std::copy(pnodes.begin() + 5, pnodes.end(), std::back_inserter(nodes_list2));
		
		REQUIRE(nodes_list.size() == 5);
		REQUIRE(nodes_list2.size() == 5);
		
		{
			auto current = nodes_list.begin();
			for(int i = 0; i < 5; ++i)
			{
				REQUIRE(*current == pnodes[i]);
				++current;
			}
		}
		
		{
			auto current = nodes_list2.begin();
			for(int i = 5; i < 10; ++i)
			{
				REQUIRE(*current == pnodes[i]);
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
			
			REQUIRE(*(current++) == pnodes[0]);
			
			for(int i = 5; i < 10; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
			}
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
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
			REQUIRE(*(current++) == pnodes[0]);
			REQUIRE(*(current++) == pnodes[6]);
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
			}
			
			current = nodes_list2.begin();
			REQUIRE(*(current++) == pnodes[5]);
			for(int i = 7; i < 10; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
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
			REQUIRE(*(current++) == pnodes[0]);
			REQUIRE(*(current++) == pnodes[6]);
			REQUIRE(*(current++) == pnodes[7]);
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
			}
			
			current = nodes_list2.begin();
			REQUIRE(*(current++) == pnodes[5]);
			for(int i = 8; i < 10; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
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
			REQUIRE(*(current++) == pnodes[6]);
			REQUIRE(*(current++) == pnodes[7]);
			REQUIRE(*(current++) == pnodes[0]);
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
			}
			
			current = nodes_list2.begin();
			REQUIRE(*(current++) == pnodes[5]);
			for(int i = 8; i < 10; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
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
			REQUIRE(*(current++) == pnodes[0]);
			for(int i = 1; i < 5; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
			}
			REQUIRE(*(current++) == pnodes[6]);
			REQUIRE(*(current++) == pnodes[7]);
			
			current = nodes_list2.begin();
			REQUIRE(*(current++) == pnodes[5]);
			for(int i = 8; i < 10; ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
			}
		}
		
		SECTION("Single self splice")
		{
			nodes_list.splice(std::next(nodes_list.begin()), nodes_list, std::next(nodes_list.begin(), 3), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == pnodes[0]);
			REQUIRE(*(current++) == pnodes[3]);
			REQUIRE(*(current++) == pnodes[1]);
			REQUIRE(*(current++) == pnodes[2]);
			REQUIRE(*(current++) == pnodes[4]);
		}
		
		SECTION("Same position self splice")
		{
			nodes_list.splice(std::next(nodes_list.begin(), 4), nodes_list, std::next(nodes_list.begin(), 3), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == pnodes[0]);
			REQUIRE(*(current++) == pnodes[1]);
			REQUIRE(*(current++) == pnodes[2]);
			REQUIRE(*(current++) == pnodes[3]);
			REQUIRE(*(current++) == pnodes[4]);
			
			nodes_list.splice(std::next(nodes_list.begin(), 3), nodes_list, std::next(nodes_list.begin(), 3), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			current = nodes_list.begin();
			REQUIRE(*(current++) == pnodes[0]);
			REQUIRE(*(current++) == pnodes[1]);
			REQUIRE(*(current++) == pnodes[2]);
			REQUIRE(*(current++) == pnodes[3]);
			REQUIRE(*(current++) == pnodes[4]);
		}

		SECTION("Previous position self splice")
		{
			nodes_list.splice(std::next(nodes_list.begin(), 2), nodes_list, std::next(nodes_list.begin(), 3), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == pnodes[0]);
			REQUIRE(*(current++) == pnodes[1]);
			REQUIRE(*(current++) == pnodes[3]);
			REQUIRE(*(current++) == pnodes[2]);
			REQUIRE(*(current++) == pnodes[4]);
		}
		
		SECTION("Multi self splice")
		{
			nodes_list.splice(std::next(nodes_list.begin()), nodes_list, std::next(nodes_list.begin(), 2), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == pnodes[0]);
			REQUIRE(*(current++) == pnodes[2]);
			REQUIRE(*(current++) == pnodes[3]);
			REQUIRE(*(current++) == pnodes[1]);
			REQUIRE(*(current++) == pnodes[4]);
		}
		
		SECTION("Self splice -> begin")
		{
			nodes_list.splice(nodes_list.begin(), nodes_list, std::next(nodes_list.begin(), 2), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == pnodes[2]);
			REQUIRE(*(current++) == pnodes[3]);
			REQUIRE(*(current++) == pnodes[0]);
			REQUIRE(*(current++) == pnodes[1]);
			REQUIRE(*(current++) == pnodes[4]);
		}
		
		SECTION("Self splice -> end")
		{
			nodes_list.splice(nodes_list.end(), nodes_list, std::next(nodes_list.begin(), 2), std::next(nodes_list.begin(), 4));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE_FALSE(nodes_list.empty());
			auto current = nodes_list.begin();
			REQUIRE(*(current++) == pnodes[0]);
			REQUIRE(*(current++) == pnodes[1]);
			REQUIRE(*(current++) == pnodes[4]);
			REQUIRE(*(current++) == pnodes[2]);
			REQUIRE(*(current++) == pnodes[3]);
		}
		
	}
}


TEST_CASE("LinkedPointersSublist")
{
	using namespace Scheduler;
	
	std::vector<Node> nodes(10);
	std::vector<Node*> pnodes(10);
	
	for(size_t i = 0; i < 10; ++i)
	{
		nodes[i].setValue(i);
		pnodes[i] = &nodes[i];
	}
	
	
	LinkedPointersList<Node*> nodes_list;
	
	SECTION("EmptyList")
	{
		LinkedPointersSublist<Node*> sublist(nodes_list, nodes_list.begin(), nodes_list.end());
		
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
		REQUIRE(sublist.empty());
		REQUIRE(sublist.size() == 0);
		
		SECTION("Push + iterate + clear")
		{
			SECTION("Push back")
			{
				std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			}
			SECTION("Push front")
			{
				std::copy(pnodes.rbegin(), pnodes.rend(), std::front_inserter(sublist));
			}

			REQUIRE(!nodes_list.empty());
			REQUIRE(nodes_list.size() == 10);
			REQUIRE(!sublist.empty());
			REQUIRE(sublist.size() == 10);
			
			auto current = sublist.begin();
			for(int i = 0; i < pnodes.size(); ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
			}
		
			auto rcurrent = sublist.rbegin();
			for(int i = 0; i < pnodes.size(); ++i)
			{
				Node* n = *(rcurrent++);
				Node* n1 = pnodes[pnodes.size() - 1 - i];
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
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
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
					REQUIRE((*nodes_list.begin())->value() == 0);
					REQUIRE((*nodes_list.rbegin())->value() == 8);
					
					REQUIRE(sublist.size() == 9);
					REQUIRE_FALSE(sublist.empty());
					REQUIRE((*sublist.begin())->value() == 0);
					REQUIRE((*sublist.rbegin())->value() == 8);
				}
				SECTION("Pop front")
				{
					sublist.pop_front();
					REQUIRE(nodes_list.size() == 9);
					REQUIRE_FALSE(nodes_list.empty());
					REQUIRE((*nodes_list.begin())->value() == 1);
					REQUIRE((*nodes_list.rbegin())->value() == 9);
					
					REQUIRE(sublist.size() == 9);
					REQUIRE_FALSE(sublist.empty());
					REQUIRE((*sublist.begin())->value() == 1);
					REQUIRE((*sublist.rbegin())->value() == 9);
				}
			}
			
			SECTION("Clear pop")
			{
				SECTION("Pop back")
				{
					for(int i = 0; i < pnodes.size(); ++i)
					{
						sublist.pop_back();
					}
				}
				
				SECTION("Pop front")
				{
					for(int i = 0; i < pnodes.size(); ++i)
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
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
			REQUIRE(nodes_list.front()->value() == 0);
			REQUIRE(nodes_list.back()->value() == 9);
			
			REQUIRE(sublist.front()->value() == 0);
			REQUIRE(sublist.back()->value() == 9);
		}
		
		SECTION("Reverse")
		{
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
		
			SECTION("Full")
			{
				sublist.reverse();
				
				auto current = sublist.begin();
				for(int i = 0; i < pnodes.size(); ++i)
				{
					Node* n = *(current++);
					Node* n1 = pnodes[pnodes.size() - 1 - i];
					REQUIRE(n == n1);
				}
			}
			SECTION("Partial")
			{
				nodes_list.reverse(std::next(sublist.begin()), std::next(sublist.begin(), 4));

				REQUIRE(*std::next(sublist.begin(), 0) == pnodes[0]);
				REQUIRE(*std::next(sublist.begin(), 1) == pnodes[3]);
				REQUIRE(*std::next(sublist.begin(), 2) == pnodes[2]);
				REQUIRE(*std::next(sublist.begin(), 3) == pnodes[1]);
				REQUIRE(*std::next(sublist.begin(), 4) == pnodes[4]);
				
				LinkedPointersList<Node*>::iterator current = std::next(sublist.begin(), 4);
				for(int i = 4; i < nodes.size(); ++i)
				{
					REQUIRE((*current)->value() == i);
					++current;
				}
			}
		}
		
		SECTION("Insert + erase")
		{
			sublist.insert(sublist.begin(), pnodes[pnodes.size()-1]);
			sublist.insert(sublist.end(), pnodes[0]);
			auto iter = sublist.insert(std::next(sublist.begin()), pnodes[pnodes.size()-2]);
			sublist.insert(iter, pnodes[1]);
			sublist.insert(iter, pnodes[2]);
			sublist.insert(iter, pnodes[3]);
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 6);
			REQUIRE(*std::next(nodes_list.begin(), 0) == pnodes[pnodes.size() - 1]);
			REQUIRE(*std::next(nodes_list.begin(), 1) == pnodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == pnodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == pnodes[3]);
			REQUIRE(*std::next(nodes_list.begin(), 4) == pnodes[pnodes.size() - 2]);
			REQUIRE(*std::next(nodes_list.begin(), 5) == pnodes[0]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 6);
			REQUIRE(*std::next(sublist.begin(), 0) == pnodes[pnodes.size() - 1]);
			REQUIRE(*std::next(sublist.begin(), 1) == pnodes[1]);
			REQUIRE(*std::next(sublist.begin(), 2) == pnodes[2]);
			REQUIRE(*std::next(sublist.begin(), 3) == pnodes[3]);
			REQUIRE(*std::next(sublist.begin(), 4) == pnodes[pnodes.size() - 2]);
			REQUIRE(*std::next(sublist.begin(), 5) == pnodes[0]);
			
			sublist.erase(std::next(sublist.begin(), 3));
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 3));
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 3);
			REQUIRE(*std::next(nodes_list.begin(), 0) == pnodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 1) == pnodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == pnodes[pnodes.size() - 2]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 3);
			REQUIRE(*std::next(sublist.begin(), 0) == pnodes[1]);
			REQUIRE(*std::next(sublist.begin(), 1) == pnodes[2]);
			REQUIRE(*std::next(sublist.begin(), 2) == pnodes[pnodes.size() - 2]);
			
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
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
			SECTION("Value")
			{
				sublist.remove(pnodes[4]);
				
				REQUIRE(nodes_list.size() == 9);
				REQUIRE(sublist.size() == 9);
				
				int current_index = 0;
				auto iter = sublist.begin();
				while(current_index < pnodes.size())
				{
					if(current_index == 4) ++current_index;
					REQUIRE(*iter == pnodes[current_index]);
					++iter;
					++current_index;
				}
			}
			
			SECTION("Predicate")
			{
				sublist.remove_if([&](Node* val){return val->value() >= 5;});
				
				REQUIRE(nodes_list.size() == 5);
				REQUIRE(sublist.size() == 5);
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*current == pnodes[i]);
					++current;
				}
			}
		}
		
		SECTION("Splice")
		{
			LinkedPointersList<Node*> nodes_list2;
			LinkedPointersSublist<Node*> sublist2(nodes_list2, nodes_list2.begin(), nodes_list2.end());
			
			std::copy(pnodes.begin(), std::next(pnodes.begin(), 5), std::back_inserter(sublist));
			std::copy(pnodes.begin() + 5, pnodes.end(), std::back_inserter(sublist2));
			
			REQUIRE(nodes_list.size() == 5);
			REQUIRE(nodes_list2.size() == 5);
			
			REQUIRE(sublist.size() == 5);
			REQUIRE(sublist2.size() == 5);
			
			{
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
			}
			
			{
				auto current = sublist2.begin();
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				
				REQUIRE(*(current++) == pnodes[0]);
				
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[6]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 7; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[6]);
				REQUIRE(*(current++) == pnodes[7]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				
				current = nodes_list2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[6]);
				REQUIRE(*(current++) == pnodes[7]);
				REQUIRE(*(current++) == pnodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				REQUIRE(*(current++) == pnodes[6]);
				REQUIRE(*(current++) == pnodes[7]);
				
				current = sublist2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[4]);
			}
			
			SECTION("Same position self splice")
			{
				sublist.splice(std::next(sublist.begin(), 4), sublist, std::next(sublist.begin(), 3), std::next(sublist.begin(), 4));
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				auto current = sublist.begin();
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[4]);
				
				sublist.splice(std::next(sublist.begin(), 3), sublist, std::next(sublist.begin(), 3), std::next(sublist.begin(), 4));
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				current = sublist.begin();
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[4]);
			}

			SECTION("Previous position self splice")
			{
				sublist.splice(std::next(sublist.begin(), 2), sublist, std::next(sublist.begin(), 3), std::next(sublist.begin(), 4));
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				auto current = sublist.begin();
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[4]);
			}
			
			SECTION("Multi self splice")
			{
				sublist.splice(std::next(sublist.begin()), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 5);
				REQUIRE_FALSE(nodes_list.empty());
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[4]);
			}
			
			SECTION("Self splice -> begin")
			{
				sublist.splice(sublist.begin(), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 5);
				REQUIRE_FALSE(nodes_list.empty());
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[4]);
			}
			
			SECTION("Self splice -> end")
			{
				sublist.splice(sublist.end(), sublist, std::next(sublist.begin(), 2), std::next(sublist.begin(), 4));
				
				REQUIRE(nodes_list.size() == 5);
				REQUIRE_FALSE(nodes_list.empty());
				
				REQUIRE(sublist.size() == 5);
				REQUIRE_FALSE(sublist.empty());
				
				auto current = sublist.begin();
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[4]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
			}
		}
	}
	
	SECTION("Middle sublist")
	{
		Node start_node;
		start_node.setValue(100);
		Node end_node;
		end_node.setValue(200);
		
		nodes_list.push_back(&end_node);
		nodes_list.push_front(&start_node);
		
		LinkedPointersSublist<Node*> sublist(nodes_list, std::next(nodes_list.begin()), std::prev(nodes_list.end()));
		
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
				std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			}
			SECTION("Push front")
			{
				std::copy(pnodes.rbegin(), pnodes.rend(), std::front_inserter(sublist));
			}

			REQUIRE(!nodes_list.empty());
			REQUIRE(nodes_list.size() == 12);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			REQUIRE(!sublist.empty());
			REQUIRE(sublist.size() == 10);
			
			auto current = sublist.begin();
			for(int i = 0; i < pnodes.size(); ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
			}
		
			auto rcurrent = sublist.rbegin();
			for(int i = 0; i < pnodes.size(); ++i)
			{
				Node* n = *(rcurrent++);
				Node* n1 = pnodes[pnodes.size() - 1 - i];
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
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
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
					REQUIRE((*sublist.begin())->value() == 0);
					REQUIRE((*sublist.rbegin())->value() == 8);
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
					REQUIRE((*sublist.begin())->value() == 1);
					REQUIRE((*sublist.rbegin())->value() == 9);
				}
			}
			
			SECTION("Clear pop")
			{
				SECTION("Pop back")
				{
					for(int i = 0; i < pnodes.size(); ++i)
					{
						sublist.pop_back();
					}
				}
				
				SECTION("Pop front")
				{
					for(int i = 0; i < pnodes.size(); ++i)
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
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			
			REQUIRE(sublist.front()->value() == 0);
			REQUIRE(sublist.back()->value() == 9);
		}
		
		SECTION("Reverse")
		{
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
			sublist.reverse();
			
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			
			auto current = sublist.begin();
			for(int i = 0; i < pnodes.size(); ++i)
			{
				Node* n = *(current++);
				Node* n1 = pnodes[pnodes.size() - 1 - i];
				REQUIRE(n == n1);
			}
		}
		
		SECTION("Insert + erase")
		{
			sublist.insert(sublist.begin(), pnodes[pnodes.size()-1]);
			sublist.insert(sublist.end(), pnodes[0]);
			auto iter = sublist.insert(std::next(sublist.begin()), pnodes[pnodes.size()-2]);
			sublist.insert(iter, pnodes[1]);
			sublist.insert(iter, pnodes[2]);
			sublist.insert(iter, pnodes[3]);
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 8);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			REQUIRE(*std::next(nodes_list.begin(), 1) == pnodes[pnodes.size() - 1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == pnodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == pnodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 4) == pnodes[3]);
			REQUIRE(*std::next(nodes_list.begin(), 5) == pnodes[pnodes.size() - 2]);
			REQUIRE(*std::next(nodes_list.begin(), 6) == pnodes[0]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 6);
			REQUIRE(*std::next(sublist.begin(), 0) == pnodes[pnodes.size() - 1]);
			REQUIRE(*std::next(sublist.begin(), 1) == pnodes[1]);
			REQUIRE(*std::next(sublist.begin(), 2) == pnodes[2]);
			REQUIRE(*std::next(sublist.begin(), 3) == pnodes[3]);
			REQUIRE(*std::next(sublist.begin(), 4) == pnodes[pnodes.size() - 2]);
			REQUIRE(*std::next(sublist.begin(), 5) == pnodes[0]);
			
			sublist.erase(std::next(sublist.begin(), 3));
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 3));
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.back() == &end_node);
			REQUIRE(nodes_list.size() == 5);
			REQUIRE(*std::next(nodes_list.begin(), 1) == pnodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == pnodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == pnodes[pnodes.size() - 2]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 3);
			REQUIRE(*std::next(sublist.begin(), 0) == pnodes[1]);
			REQUIRE(*std::next(sublist.begin(), 1) == pnodes[2]);
			REQUIRE(*std::next(sublist.begin(), 2) == pnodes[pnodes.size() - 2]);
			
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
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
			SECTION("Value")
			{
				sublist.remove(pnodes[4]);
				
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				REQUIRE(nodes_list.size() == 11);
				REQUIRE(sublist.size() == 9);
				
				int current_index = 0;
				auto iter = sublist.begin();
				while(current_index < pnodes.size())
				{
					if(current_index == 4) ++current_index;
					REQUIRE(*iter == pnodes[current_index]);
					++iter;
					++current_index;
				}
			}
			
			SECTION("Predicate")
			{
				sublist.remove_if([&](Node* val){return val->value() >= 5;});
				
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.back() == &end_node);
				REQUIRE(nodes_list.size() == 7);
				REQUIRE(sublist.size() == 5);
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*current == pnodes[i]);
					++current;
				}
			}
		}
		
		SECTION("Splice")
		{
			LinkedPointersList<Node*> nodes_list2;
			LinkedPointersSublist<Node*> sublist2(nodes_list2, nodes_list2.begin(), nodes_list2.end());
			
			std::copy(pnodes.begin(), std::next(pnodes.begin(), 5), std::back_inserter(sublist));
			std::copy(pnodes.begin() + 5, pnodes.end(), std::back_inserter(sublist2));
			
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
					REQUIRE(*(current++) == pnodes[i]);
				}
			}
			
			{
				auto current = sublist2.begin();
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				
				REQUIRE(*(current++) == pnodes[0]);
				
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[6]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 7; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[6]);
				REQUIRE(*(current++) == pnodes[7]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				
				current = nodes_list2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[6]);
				REQUIRE(*(current++) == pnodes[7]);
				REQUIRE(*(current++) == pnodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				REQUIRE(*(current++) == pnodes[6]);
				REQUIRE(*(current++) == pnodes[7]);
				
				current = sublist2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[4]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[4]);
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
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[4]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[4]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
			}
		}
	}

	SECTION("Tail sublist")
	{
		Node start_node;
		start_node.setValue(100);
		
		nodes_list.push_front(&start_node);
		
		LinkedPointersSublist<Node*> sublist(nodes_list, std::next(nodes_list.begin()), nodes_list.end());
		
		REQUIRE_FALSE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 1);
		REQUIRE(nodes_list.front() == &start_node);
		REQUIRE(sublist.empty());
		REQUIRE(sublist.size() == 0);
		
		SECTION("Push + iterate + clear")
		{
			SECTION("Push back")
			{
				std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			}
			SECTION("Push front")
			{
				std::copy(pnodes.rbegin(), pnodes.rend(), std::front_inserter(sublist));
			}

			REQUIRE(!nodes_list.empty());
			REQUIRE(nodes_list.size() == 11);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(!sublist.empty());
			REQUIRE(sublist.size() == 10);
			
			auto current = sublist.begin();
			for(int i = 0; i < pnodes.size(); ++i)
			{
				REQUIRE(*(current++) == pnodes[i]);
			}
		
			auto rcurrent = sublist.rbegin();
			for(int i = 0; i < pnodes.size(); ++i)
			{
				Node* n = *(rcurrent++);
				Node* n1 = pnodes[pnodes.size() - 1 - i];
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
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
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
					REQUIRE((*sublist.begin())->value() == 0);
					REQUIRE((*sublist.rbegin())->value() == 8);
				}
				SECTION("Pop front")
				{
					sublist.pop_front();
					REQUIRE(nodes_list.size() == 10);
					REQUIRE_FALSE(nodes_list.empty());
					REQUIRE(nodes_list.front() == &start_node);
					
					REQUIRE(sublist.size() == 9);
					REQUIRE_FALSE(sublist.empty());
					REQUIRE((*sublist.begin())->value() == 1);
					REQUIRE((*sublist.rbegin())->value() == 9);
				}
			}
			
			SECTION("Clear pop")
			{
				SECTION("Pop back")
				{
					for(int i = 0; i < pnodes.size(); ++i)
					{
						sublist.pop_back();
					}
				}
				
				SECTION("Pop front")
				{
					for(int i = 0; i < pnodes.size(); ++i)
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
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
			REQUIRE(nodes_list.front() == &start_node);
			
			REQUIRE(sublist.front()->value() == 0);
			REQUIRE(sublist.back()->value() == 9);
		}
		
		SECTION("Reverse")
		{
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
			sublist.reverse();
			
			REQUIRE(nodes_list.front() == &start_node);
			
			auto current = sublist.begin();
			for(int i = 0; i < pnodes.size(); ++i)
			{
				Node* n = *(current++);
				Node* n1 = pnodes[pnodes.size() - 1 - i];
				REQUIRE(n == n1);
			}
		}
		
		SECTION("Insert + erase")
		{
			sublist.insert(sublist.begin(), pnodes[pnodes.size()-1]);
			sublist.insert(sublist.end(), pnodes[0]);
			auto iter = sublist.insert(std::next(sublist.begin()), pnodes[pnodes.size()-2]);
			sublist.insert(iter, pnodes[1]);
			sublist.insert(iter, pnodes[2]);
			sublist.insert(iter, pnodes[3]);
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.size() == 7);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(*std::next(nodes_list.begin(), 1) == pnodes[pnodes.size() - 1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == pnodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == pnodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 4) == pnodes[3]);
			REQUIRE(*std::next(nodes_list.begin(), 5) == pnodes[pnodes.size() - 2]);
			REQUIRE(*std::next(nodes_list.begin(), 6) == pnodes[0]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 6);
			REQUIRE(*std::next(sublist.begin(), 0) == pnodes[pnodes.size() - 1]);
			REQUIRE(*std::next(sublist.begin(), 1) == pnodes[1]);
			REQUIRE(*std::next(sublist.begin(), 2) == pnodes[2]);
			REQUIRE(*std::next(sublist.begin(), 3) == pnodes[3]);
			REQUIRE(*std::next(sublist.begin(), 4) == pnodes[pnodes.size() - 2]);
			REQUIRE(*std::next(sublist.begin(), 5) == pnodes[0]);
			
			sublist.erase(std::next(sublist.begin(), 3));
			sublist.erase(std::next(sublist.begin(), 0));
			sublist.erase(std::next(sublist.begin(), 3));
			
			REQUIRE_FALSE(nodes_list.empty());
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list.size() == 4);
			REQUIRE(*std::next(nodes_list.begin(), 1) == pnodes[1]);
			REQUIRE(*std::next(nodes_list.begin(), 2) == pnodes[2]);
			REQUIRE(*std::next(nodes_list.begin(), 3) == pnodes[pnodes.size() - 2]);
			
			REQUIRE_FALSE(sublist.empty());
			REQUIRE(sublist.size() == 3);
			REQUIRE(*std::next(sublist.begin(), 0) == pnodes[1]);
			REQUIRE(*std::next(sublist.begin(), 1) == pnodes[2]);
			REQUIRE(*std::next(sublist.begin(), 2) == pnodes[pnodes.size() - 2]);
			
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
			std::copy(pnodes.begin(), pnodes.end(), std::back_inserter(sublist));
			
			SECTION("Value")
			{
				sublist.remove(pnodes[4]);
				
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.size() == 10);
				REQUIRE(sublist.size() == 9);
				
				int current_index = 0;
				auto iter = sublist.begin();
				while(current_index < pnodes.size())
				{
					if(current_index == 4) ++current_index;
					REQUIRE(*iter == pnodes[current_index]);
					++iter;
					++current_index;
				}
			}
			
			SECTION("Predicate")
			{
				sublist.remove_if([&](Node* val){return val->value() >= 5;});
				
				REQUIRE(nodes_list.front() == &start_node);
				REQUIRE(nodes_list.size() == 6);
				REQUIRE(sublist.size() == 5);
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*current == pnodes[i]);
					++current;
				}
			}
		}
		
		SECTION("Splice")
		{
			LinkedPointersList<Node*> nodes_list2;
			LinkedPointersSublist<Node*> sublist2(nodes_list2, nodes_list2.begin(), nodes_list2.end());
			
			std::copy(pnodes.begin(), std::next(pnodes.begin(), 5), std::back_inserter(sublist));
			std::copy(pnodes.begin() + 5, pnodes.end(), std::back_inserter(sublist2));
			
			REQUIRE(nodes_list.size() == 6);
			REQUIRE(nodes_list.front() == &start_node);
			REQUIRE(nodes_list2.size() == 5);
			
			REQUIRE(sublist.size() == 5);
			REQUIRE(sublist2.size() == 5);
			
			{
				auto current = sublist.begin();
				for(int i = 0; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
			}
			
			{
				auto current = sublist2.begin();
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				
				REQUIRE(*(current++) == pnodes[0]);
				
				for(int i = 5; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[6]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 7; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[6]);
				REQUIRE(*(current++) == pnodes[7]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				
				current = nodes_list2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[6]);
				REQUIRE(*(current++) == pnodes[7]);
				REQUIRE(*(current++) == pnodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				
				current = sublist2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				for(int i = 1; i < 5; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
				}
				REQUIRE(*(current++) == pnodes[6]);
				REQUIRE(*(current++) == pnodes[7]);
				
				current = sublist2.begin();
				REQUIRE(*(current++) == pnodes[5]);
				for(int i = 8; i < 10; ++i)
				{
					REQUIRE(*(current++) == pnodes[i]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[4]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[4]);
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
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[4]);
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
				REQUIRE(*(current++) == pnodes[0]);
				REQUIRE(*(current++) == pnodes[1]);
				REQUIRE(*(current++) == pnodes[4]);
				REQUIRE(*(current++) == pnodes[2]);
				REQUIRE(*(current++) == pnodes[3]);
			}
		}
	}
}


TEST_CASE("Nested sublists")
{
	using namespace Scheduler;
	
	std::vector<Node> nodes(10);
	std::vector<Node*> pnodes(10);
	
	for(size_t i = 0; i < 10; ++i)
	{
		nodes[i].setValue(i);
		pnodes[i] = &nodes[i];
	}
	
	LinkedPointersList<Node*> nodes_list;
	
	nodes_list.push_front(pnodes[0]);
	nodes_list.push_back(pnodes[8]);
	nodes_list.push_back(pnodes[9]);
	
	LinkedPointersSublist<Node*> sublist1(nodes_list, std::next(nodes_list.begin()), std::prev(nodes_list.end()));
	
	sublist1.push_front(pnodes[7]);	
	sublist1.push_front(pnodes[6]);
	sublist1.push_front(pnodes[1]);
	
	LinkedPointersSublist<Node*, LinkedPointersSublist<Node*>> sublist2(sublist1, std::next(sublist1.begin()), std::next(sublist1.begin(), 2));
	
	sublist2.push_front(pnodes[5]);
	sublist2.push_front(pnodes[4]);
	sublist2.push_front(pnodes[3]);
	sublist2.push_front(pnodes[2]);
	
	REQUIRE(nodes_list.size() == 10);
	REQUIRE(sublist1.size() == 8);
	REQUIRE(sublist2.size() == 5);
	
	LinkedPointersList<Node*>::iterator current = nodes_list.begin();
	for(int i = 0; i < nodes.size(); ++i)
	{
		REQUIRE((*current)->value() == i);
		++current;
	}
	
	sublist2.clear();
	REQUIRE(nodes_list.size() == 5);
	REQUIRE(sublist1.size() == 3);
	
	sublist1.clear();
	REQUIRE(nodes_list.size() == 2);
	
}