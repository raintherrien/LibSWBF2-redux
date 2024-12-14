#include "pch.h"
#include "Logger.h"
#include "FileWriter.h"
#include "InternalHelpers.h"

namespace LibSWBF2::Logging
{
	Logger::Logger()
	{
		m_Writer.Open(LOG_FILE, true);
	}

	Logger::~Logger()
	{
		m_Writer.Close();
	}

	Logger &Logger::GetInstance()
	{
		static Logger singleton;
		return singleton;
	}

	void Logger::SetLogfileLevel(ELogType LogfileLevel)
	{
		GetInstance().m_LogfileLevel = LogfileLevel;
	}

	void Logger::Log(
		const std::string &message,
		ELogType level,
		unsigned long line,
		const char *file,
		const char *func
	) noexcept {
		try {
			Logger &instance = GetInstance();
			if (message.size() > 0 && level >= instance.m_LogfileLevel)
			{
				LoggerEntry entry(message, level, line, file, func);
				instance.m_Writer.WriteLine(entry.ToString());
			}
		} catch (...) {
			// We cannot throw!
		}
	}
}
