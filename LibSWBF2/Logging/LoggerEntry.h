#pragma once
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Logging
{
	struct LoggerEntry
	{
		LoggerEntry() = default;
		LoggerEntry(const std::string &message, ELogType level, unsigned long line, const char *file, const char *func);
		std::string ToString() const;

		std::string m_Message;
		ELogType m_Level;
		unsigned long m_Line;
		std::string m_File;
		std::string m_Func;
	};
}
