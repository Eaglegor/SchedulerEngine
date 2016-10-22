#pragma once

#include <string>

#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>

#include "SceneObjectsFactory.h"
#include "Constraints/Order/OrderConstraints.h"

#include <Engine/Utils/String.h>
#include <Engine/Utils/Optional.h>
#include <boost/optional.hpp>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Operation;
	class Attribute;
	class Location;
	class Depot;

    class SCENEMANAGER_EXPORT Order
    {
    public:
		struct Context
		{
			SceneObjectsFactory<Operation> &operations_factory;
		};
		
		Order(std::size_t id, const Context& context, Optional<const Depot&> depot);
		~Order();

        std::size_t getId() const;

        const String& getName() const;
		void setName(const String& name);

		Optional<const Operation&> getStartOperation() const;
		Optional<const Operation&> getWorkOperation() const;
        Optional<const Operation&> getEndOperation() const;

		Optional<const Depot&> getDepot() const;

		Operation& createStartOperation(const Location& location);
		Operation& createWorkOperation(const Location& location);
		Operation& createEndOperation(const Location& location);

		const OrderConstraints& constraints() const;
		OrderConstraints& constraints();

		const Context& getContext() const;
		
		bool operator==(const Order& rhs) const;
		bool operator!=(const Order& rhs) const;

    private:
        std::size_t id;
		Context context;
		
        String name;

        Optional<Operation&> start_operation;
        Optional<Operation&> work_operation;
        Optional<Operation&> end_operation;
		
		Optional<const Depot&> depot;

		OrderConstraints order_constraints;
    };
}