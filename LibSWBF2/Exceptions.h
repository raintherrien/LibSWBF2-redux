#pragma once
#include <stdexcept>
#include <string>

namespace LibSWBF2
{
	class LibSWBF2Exception : public std::runtime_error
	{
	public:
		LibSWBF2Exception(const char *msg) : std::runtime_error(msg) {}
		LibSWBF2Exception(const std::string& msg) : std::runtime_error(msg) {}
	};
}
