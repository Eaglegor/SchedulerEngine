#pragma once

#include <string>

#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>

#include "SceneObjectsFactory.h"
#include "Constraints/Order/OrderConstraints.h"

#include <SceneManager_export.h>

namespace Scheduler
{
	class Operation;
	class Attribute;
	class Location;

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
    class SCENEMANAGER_EXPORT Order
    {
    public:
		Order(size_t id);
		~Order();

        std::size_t getId() const;

        const char* getName() const;
		Operation* getStartOperation();
        const Operation* getStartOperation() const;
        Operation* getWorkOperation();
		const Operation* getWorkOperation() const;
		Operation* getEndOperation();
        const Operation* getEndOperation() const;

		void setName(const char* name);

		Operation* createStartOperation(const Location& location);
		Operation* createWorkOperation(const Location& location);
		Operation* createEndOperation(const Location& location);

		const OrderConstraints& constraints() const;
		OrderConstraints& constraints();

		// == framework internal ====================================
		void setOperationsFactory(SceneObjectsFactory<Operation> *factory);

    private:
        size_t id;
        std::string name;

        Operation* start_operation;
        Operation* work_operation;
        Operation* end_operation;

		SceneObjectsFactory<Operation> *operations_factory;

		OrderConstraints order_constraints;
    };
}