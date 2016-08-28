#include "Patch.h"
#include "Action.h"

namespace Scheduler
{
	Patch::Patch():
	state(State::NOT_INITIALIZED),
	base_version(0),
	current_version(0),
	memory_manager(nullptr)
	{}
	
	Patch::Patch(Patch&& rhs):
	state(rhs.state),
	base_version(rhs.base_version),
	current_version(rhs.current_version),
	memory_manager(rhs.memory_manager),
	actions(rhs.actions)
	{
		rhs.state = State::NOT_INITIALIZED;
		rhs.base_version = 0;
		rhs.current_version = 0;
		rhs.memory_manager = nullptr;
	}

	Patch::~Patch()
	{
		if(state == State::OPEN)
		{
			STATIC_SIMPLE_LOG_WARNING("Patch", "Destroying patch in open state. Rollback will be performed");
			hold();
		}
		actions.clear();
	}
	
	std::size_t Patch::getBaseVersion()
	{
		return base_version;
	}

	std::size_t Patch::getCurrentVersion()
	{
		return current_version;
	}
	
	Patch::State Patch::getState()
	{
		return state;
	}
	
	void Patch::hold()
	{
		for(auto iter = actions.rbegin(); iter != actions.rend(); ++iter)
		{
			iter->get()->rollback();
		}
		state = State::HELD;
	}
	
	void Patch::recall()
	{
		for(auto iter = actions.begin(); iter != actions.end(); ++iter)
		{
			iter->get()->perform();
		}
		state = State::OPEN;
	}

	void Patch::initialize(MemoryManager* memory_manager, std::size_t base_version)
	{
		this->memory_manager = memory_manager;
		this->base_version = base_version;
		this->current_version = base_version;
		state = State::OPEN;
	}
	
	bool Patch::operator=(Patch&& rhs)
	{
		this->state = rhs.state;
		this->base_version = rhs.base_version;
		this->current_version = rhs.current_version;
		this->memory_manager = rhs.memory_manager;
		this->actions = std::move(rhs.actions);
		
		rhs.state = State::NOT_INITIALIZED;
		rhs.base_version = 0;
		rhs.current_version = 0;
		rhs.memory_manager = nullptr;
	}

	
}