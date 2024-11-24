#pragma once
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Logging
{
	struct LoggerEntry
	{
		LoggerEntry() = default;
		LoggerEntry(const char* message, const ELogType level, const uint32_t line, const char* file);
		LIBSWBF2_API std::string ToString() const;

		std::string m_Message;
		ELogType m_Level;
		uint32_t m_Line;
		std::string m_File;
	};
}
