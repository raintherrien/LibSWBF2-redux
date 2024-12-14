#pragma once
#include "LoggerEntry.h"
#include "Chunks/HeaderNames.h"
#include "Chunks/BaseChunk.h"
#include "IO/FileWriter.h"

namespace LibSWBF2::Logging
{
	// Singleton
	class Logger
	{
	public:
		LIBSWBF2_API static void SetLogfileLevel(ELogType LogfileLevel);

		static void Log(
			const std::string &message,
			ELogType level,
			unsigned long line,
			const char *file,
			const char *func
		) noexcept;

	private:
		Logger(); // do not call anywhere else
		~Logger();

		static Logger &GetInstance();

		static constexpr char *LOG_FILE = "LibSWBF2.log";

		FileWriter m_Writer;
		ELogType m_LogfileLevel = ELogType::Info;
	};
}
