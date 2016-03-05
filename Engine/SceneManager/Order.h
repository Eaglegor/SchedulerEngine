#pragma once

#include <string>

#include <Utils/Collections/ImmutableVector.h>
#include <Utils/Collections/ImmutableUnorderedSet.h>

#include "ConceptFactory.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class Operation;
	class Attribute;

    class SCHEDULERENGINE_EXPORT Order
    {
    public:
		Order(size_t id);
		~Order();

        size_t getId() const;

        const char* getName() const;
		Operation* getStartOperation();
        const Operation* getStartOperation() const;
        ImmutableVector<Operation*>& getWorkOperations();
		const ImmutableVector<Operation*>& getWorkOperations() const;
		Operation* getEndOperation();
        const Operation* getEndOperation() const;
        ImmutableUnorderedSet<const Attribute*>& getVehicleRequirements() const;
		ImmutableUnorderedSet<const Attribute*>& getPerformerSkillsRequirements() const;

		void setName(const char* name);

		Operation* createStartOperation();
		Operation* createWorkOperation();
		Operation* createEndOperation();

		void setVehicleRequirements(const ImmutableUnorderedSet<const Attribute*> &attributes);
		void setPerformerSkillsRequirements(const ImmutableUnorderedSet<const Attribute*> &attributes);

		void setOperationsFactory(ConceptFactory<Operation> *factory);

    private:
        size_t id;
        std::string name;

        Operation* start_operation;
        std::vector<Operation*> work_operations;
        Operation* end_operation;

		std::unordered_set<const Attribute*> vehicle_requirements;
		std::unordered_set<const Attribute*> performer_skills_requirements;

		ConceptFactory<Operation> *operations_factory;
    };
}