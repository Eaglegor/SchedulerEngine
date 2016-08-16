#include <catch.hpp>

#include <Engine/Utils/Collections/LinkedPointersList.h>
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
		
		nodes_list.reverse();
		
		LinkedPointersList<Node*>::iterator current = nodes_list.begin();
		for(int i = 0; i < nodes.size(); ++i)
		{
			REQUIRE((*current)->value() == nodes.size() - 1 - i);
			++current;
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
