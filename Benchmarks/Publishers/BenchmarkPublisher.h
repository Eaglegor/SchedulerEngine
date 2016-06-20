#pragma once

#include "BenchmarkResult.h"

namespace Scheduler
{
	class BenchmarkPublisher
	{
	public:
		virtual ~BenchmarkPublisher(){}
		virtual void addResult(const BenchmarkResult& result) = 0;

		virtual void publish() = 0;
	};
}