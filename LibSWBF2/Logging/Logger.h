#pragma once
#include "LoggerEntry.h"
#include "Types/LibString.h"
#include "Chunks/HeaderNames.h"
#include "Chunks/BaseChunk.h"


namespace LibSWBF2
{
	class FileWriter;
}

namespace LibSWBF2::Logging
{
	// Singleton
	class Logger
	{
	public:
		Logger(); // do not call anywhere else
		~Logger();

		LIBSWBF2_API static void SetLogfileLevel(const ELogType LogfileLevel);

		static void Log(const std::string &message, const ELogType level, const unsigned long line, const char* file) noexcept;

	private:
		static Logger &GetInstance();

		static constexpr char *LOG_FILE = "LibSWBF2.log";
		static const int MAX_ENTRIES = 10000;

		FileWriter* m_Writer;
		ELogType m_LogfileLevel = ELogType::Warning;
	};
}
