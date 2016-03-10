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

	/** 
		Class representing single order. All order operations must be planned to a single vehicle run.

		Contains 3 types of operations:
		- start operation [0..1]: operation which must be done before the vehicle leaves it's start point (e.g. loading in depot for deliveries)
		- work operations [0..*]: operations which must be done on client sites (e.g. delivering order to the customer or collecting the package from him)
		- end_operation [0..1]: operation which must be done after the vehicle reaches it's end point (e.g. unloading packages, collected from the customers)

		Each order may have vehicle requirements (to be able to complete the order vehicle must have the same or larger attributes set) and 
		performer skills requirements (performer must have the same or larger skills set to be able to complete this order).

		The total load of all operations must be zero (representing that no goods are leaved in a vehicle after the order is finished and that no goods appear magically in the vehicle).
	*/
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

		// == framework internal ====================================
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