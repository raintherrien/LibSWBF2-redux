#include "pch.h"
#include "Logger.h"
#include "FileWriter.h"
#include "InternalHelpers.h"

namespace LibSWBF2::Logging
{
	Logger::Logger()
	{
		m_Writer = new FileWriter();
		m_Writer->Open(LOG_FILE, true);
	}

	Logger::~Logger()
	{
		m_Writer->Close();
		delete m_Writer;
	}

	Logger &Logger::GetInstance()
	{
		static Logger singleton;
		return singleton;
	}

	void Logger::SetLogfileLevel(const ELogType LogfileLevel)
	{
		GetInstance().m_LogfileLevel = LogfileLevel;
	}

	void Logger::Log(const std::string &message, const ELogType level, const unsigned long line, const char* file) noexcept
	{
		try {
			Logger &instance = GetInstance();
			if (message.length() > 0 && level >= instance.m_LogfileLevel)
			{
				LoggerEntry entry(message.c_str(), level, line, file);
				instance.m_Writer->WriteLine(entry.ToString());
			}
		} catch (...) {
			// We cannot throw!
		}
	}
}
