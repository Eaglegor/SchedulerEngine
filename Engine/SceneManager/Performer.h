#pragma once

#include <cstddef>
#include <string>
#include <Engine/Concepts/TimeWindow.h>
#include <Engine/Concepts/Cost.h>

#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <Engine/Utils/String.h>
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include "Attribute.h"

#include "Constraints/Performer/PerformerConstraints.h"

#include <SceneManager_export.h>

namespace Scheduler
{
    class SCENEMANAGER_EXPORT Performer
    {
    public:
		using Skill = Attribute;
		using SkillsSet = std::unordered_set<ReferenceWrapper<const Skill>>;
		
		Performer(std::size_t id);

		std::size_t getId() const;
		
		const String& getName() const;
		void setName(const String& name);

		const Cost& getDurationUnitCost() const;
		void setDurationUnitCost(const Cost &cost);
		
		const Cost& getActivationCost() const;
		void setActivationCost(const Cost &cost);

		const SkillsSet& getSkills() const;
		void addSkill(const Skill& skill);
		void removeSkill(const Skill& skill);
		void clearSkills();

		const PerformerConstraints& constraints() const;
		PerformerConstraints& constraints();
		
		bool operator==(const Performer& rhs) const;
		bool operator!=(const Performer& rhs) const;

	private:
		std::size_t id;
		String name;
		
		Cost duration_unit_cost;
		Cost activation_cost;

		SkillsSet skills;

		PerformerConstraints performer_constraints;
	};
}
