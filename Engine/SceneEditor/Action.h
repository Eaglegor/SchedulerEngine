#pragma once

namespace Scheduler
{
	class Action
	{
	public:
		virtual ~Action(){}

		virtual void perform() = 0;
		virtual void rollback() = 0;
	};
}