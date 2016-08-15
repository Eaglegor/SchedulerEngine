#include <catch.hpp>

#include <Engine/Utils/Collections/LinkedPointersList.h>
#include <cstddef>
#include <vector>

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

TEST_CASE("LinkedPointersList", "")
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
		REQUIRE(nodes_list.max_size() == std::numeric_limits<size_t>::max());
	}
	
	SECTION("Push back + iterate + clear")
	{
		for(Node& node : nodes)
		{
			nodes_list.push_back(&node);
		}
		REQUIRE(!nodes_list.empty());
		REQUIRE(nodes_list.size() == 10);
		LinkedPointersList<Node*>::iterator current = nodes_list.begin();
		for(int i = 0; i < nodes.size(); ++i)
		{
			std::cout << (*current)->value() << std::endl;
			//REQUIRE(current->value() == i);
			++current;
		}
		
		nodes_list.clear();
		
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
	}
	
	SECTION("Push front + reverse_iterate")
	{
		for(Node& node : nodes)
		{
			nodes_list.push_front(&node);
		}
		REQUIRE(!nodes_list.empty());
		REQUIRE(nodes_list.size() == 10);
		LinkedPointersList<Node*>::reverse_iterator current = nodes_list.rbegin();
		for(int i = 0; i < nodes.size(); ++i)
		{
			std::cout << (*current)->value() << std::endl;
			//REQUIRE( ((*current)->value()) == i);
			++current;
		}
		
		nodes_list.clear();
		
		REQUIRE(nodes_list.empty());
		REQUIRE(nodes_list.size() == 0);
	}
	
}
