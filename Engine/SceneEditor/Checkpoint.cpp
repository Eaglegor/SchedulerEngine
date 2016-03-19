#include "Checkpoint.h"

namespace Scheduler
{
	void Checkpoint::rollback()
	{
		for (auto iter = actions.rbegin(); iter != actions.rend(); ++iter)
		{
			iter->get()->rollback();
		}
		actions.clear();
	}

	void Checkpoint::clear()
	{
		actions.clear();
	}
}