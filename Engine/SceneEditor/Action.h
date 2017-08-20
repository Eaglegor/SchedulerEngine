#pragma once

namespace Scheduler
{
	class ActionBase
	{
	public:
		virtual ~ActionBase( )
		{
		}

		virtual void quiet_perform( ) = 0;
		virtual void rollback( )      = 0;
	};

	template <typename RetVal>
	class Action : public ActionBase
	{
	public:
		using return_type = RetVal;

		virtual void quiet_perform( ) override final
		{
			perform( );
		}

		virtual RetVal perform( ) = 0;
	};
}