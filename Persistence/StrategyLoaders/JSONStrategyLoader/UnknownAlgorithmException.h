#pragma once

#include <Engine/Utils/String.h>
#include <stdexcept>

namespace Scheduler
{
	class UnknownAlgorithmException : public std::logic_error
	{
	public:
		explicit UnknownAlgorithmException(const String& algorithm_name)
		    : logic_error("Unknown algorithm"),
		      algorithm_name(algorithm_name)
		{
		}

	private:
		String algorithm_name;
	};
}