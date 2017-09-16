#pragma once

#include "Stop.h"
#include <Engine/Utils/Collections/RangeTypeAdapter.h>

#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>


namespace Scheduler
{
	class WorkOperation;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Represents work at customer
	 */
	class WorkStop : public Stop
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param id Identidier of the stop
		 * @param context Internal stop context
		 * @param run Run the stop belongs to
		 * @param operation Operation that has to be performed
		 */
		WorkStop(std::size_t id, const Stop::Context& context, Run& run, const WorkOperation& operation);
		WorkStop(const WorkStop& rhs) = delete;

		/**
		 * @brief Returns the identifier of the stop
		 * 
		 * @return Identifier of the stop
		 */
		std::size_t getId( );

		/**
		 * @brief Returns the operation that has to be performed
		 * 
		 * @return Operation that has to be performed
		 */
		const WorkOperation& getOperation( ) const;

		/**
		 * @brief Returns the location of the stop
		 * 
		 * @return Location of the stop
		 */
		virtual const Location& getLocation( ) const override;

		/**
		 * @brief Accepts the stop visitor
		 * 
		 * @param Visitor to accept
		 */
		void acceptVisitor(StopVisitor& visitor) override;

		/**
		 * @brief Accepts the stop visitor
		 * 
		 * @param Visitor to accept
		 */
		void acceptVisitor(ConstStopVisitor& visitor) const override;

		/**
		 * @brief Equality operator
		 * 
		 * @return @c true, if @rhs is the same object as @c this, @c false otherwise
		 */
		bool operator==(const WorkStop& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @return Result of !(rhs == *this)
		 */
		bool operator!=(const WorkStop& rhs) const;

	private:
		std::size_t id;
		const WorkOperation& operation;
	};
}
