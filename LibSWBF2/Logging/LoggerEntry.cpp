#include "pch.h"
#include "LoggerEntry.h"

namespace LibSWBF2::Logging
{
	LoggerEntry::LoggerEntry(const char *message, ELogType level, unsigned long line, const char *file, const char *func)
	{
		this->m_Message = message;
		this->m_Level = level;
		this->m_Line = line;
		this->m_File = file;
		this->m_Func = func;
	}

	std::string LoggerEntry::ToString() const
	{
		return "[" + LogTypeToString(m_Level) + "] " + m_File + ":" + std::to_string(m_Line) + ":" + m_Func + " " + m_Message;
	}
}
