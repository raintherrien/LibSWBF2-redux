#include "stdafx.h"
#include "API.h"

namespace LibSWBF2::API
{
	LIBSWBF2_API MSH* MSH_LoadFromFile(const char* path)
	{
		return new MSH();
	}

	LIBSWBF2_API void MSH_Delete(MSH* ptr)
	{
		delete ptr;
	}

	// Logging //
	LIBSWBF2_API void AddLogMessage(const char* message, const ELogType logType)
	{
		LOG("[API] " + string(message), logType);
	}
}