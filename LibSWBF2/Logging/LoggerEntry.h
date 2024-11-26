#pragma once
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Logging
{
	struct LoggerEntry
	{
		LoggerEntry() = default;
		LoggerEntry(const char *message, ELogType level, unsigned long line, const char *file, const char *func);
		LIBSWBF2_API std::string ToString() const;

		std::string m_Message;
		ELogType m_Level;
		unsigned long m_Line;
		std::string m_File;
		std::string m_Func;
	};
}
