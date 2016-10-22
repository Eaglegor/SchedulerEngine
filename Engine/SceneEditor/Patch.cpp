#include "Patch.h"
#include "Action.h"

namespace Scheduler
{
	Patch::Patch():
	state(State::NOT_INITIALIZED),
	base_version(0),
	current_version(0),
	logger(LoggingManager::getLogger("Patch"))
	{}
	
	Patch::Patch(Patch&& rhs):
	state(rhs.state),
	base_version(rhs.base_version),
	current_version(rhs.current_version),
	actions(rhs.actions),
	logger(LoggingManager::getLogger("Patch"))
	{
		rhs.state = State::NOT_INITIALIZED;
		rhs.base_version = 0;
		rhs.current_version = 0;
	}

	Patch::~Patch()
	{
		if(state == State::OPEN)
		{
			logger.warn("Destroying patch in open state. Rollback will be performed");
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
			iter->get()->quiet_perform();
		}
		state = State::OPEN;
	}

	void Patch::initialize(std::size_t base_version)
	{
		this->base_version = base_version;
		this->current_version = base_version;
		state = State::OPEN;
	}
	
	Patch& Patch::operator=(Patch&& rhs)
	{
		this->state = rhs.state;
		this->base_version = rhs.base_version;
		this->current_version = rhs.current_version;
		this->actions = std::move(rhs.actions);
		
		rhs.state = State::NOT_INITIALIZED;
		rhs.base_version = 0;
		rhs.current_version = 0;

		return *this;
	}

	
}