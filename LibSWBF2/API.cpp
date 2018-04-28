#include "stdafx.h"
#include "API.h"
#include "Color.h"
#include "Logger.h"
#include <string>

namespace LibSWBF2
{
	using std::string;

	LIBSWBF2_API Color* Color_Create(const float r, const float g, const float b, const float a)
	{
		return new Color(r, g, b, a);
	}

	LIBSWBF2_API void Color_Delete(Color* color)
	{
		delete color;
	}

	LIBSWBF2_API void AddLogMessage(const char* message, const ELogType logType)
	{
		Logger::Add("[API] " + string(message), logType);
	}

	LIBSWBF2_API void GetLogMessages(char* messages, const int length, const ELogType logType)
	{
		string lines = Logger::GetAllLines(logType);
		strcpy_s(messages, length, lines.c_str());
	}

	LIBSWBF2_API void GetLastLogMessages(char* messages, const int length, const ELogType logType)
	{
		string lines = Logger::GetLastLines(logType);
		strcpy_s(messages, length, lines.c_str());
	}
}