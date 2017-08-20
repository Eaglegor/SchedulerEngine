#include "Performer.h"

namespace Scheduler {
    Performer::Performer(std::size_t id):
    id(id)
    {
    }

    std::size_t Performer::getId() const 
    {
        return id;
    }

    const String& Performer::getName() const 
    {
        return name;
    }

	void Performer::setName(const String& name) 
	{
        this->name = name;
    }

	const Cost& Performer::getDurationUnitCost() const 
	{
		return duration_unit_cost;
	}

	const Cost& Performer::getActivationCost() const 
	{
		return activation_cost;
	}

	void Performer::setDurationUnitCost(const Cost &cost) 
	{
		duration_unit_cost = cost;
	}

	void Performer::setActivationCost(const Cost &cost) 
	{
		activation_cost = cost;
	}

	const Performer::SkillsSet& Performer::getSkills() const
	{
		return skills;
	}

	void Performer::addSkill(const Performer::Skill& skill)
	{
		skills.emplace(skill);
	}
	
	void Performer::removeSkill(const Performer::Skill& skill)
	{
		skills.erase(skill);
	}
	
	void Performer::clearSkills()
	{
		skills.clear();
	}
	
	const PerformerConstraints & Performer::constraints() const
	{
		return performer_constraints;
	}
	
	PerformerConstraints & Performer::constraints()
	{
		return performer_constraints;
	}
	
	bool Performer::operator==(const Performer& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}
	
	bool Performer::operator!=(const Performer& rhs) const
	{
		return !(*this == rhs);
	}

}

