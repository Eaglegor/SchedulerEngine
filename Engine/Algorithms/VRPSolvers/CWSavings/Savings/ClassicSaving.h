#pragma once

class Order;

namespace Scheduler
{
	class ClassicSaving
	{
		public:
			ClassicSaving():i(nullptr),j(nullptr),saving(0){}
			
			bool operator<(const ClassicSaving& rhs) const {return saving < rhs.saving;}
			bool operator>(const ClassicSaving& rhs) const {return saving > rhs.saving;}
			
			Order* i;
			Order* j;
			float saving;
	};
}