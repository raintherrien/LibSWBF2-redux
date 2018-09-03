#pragma once
#include "Logger.h"
#include "LoggerEntry.h"
#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "MSH.h"

#ifdef LIBSWBF2_EXPORTS
#define LIBSWBF2_API __declspec(dllexport)
#else
#define LIBSWBF2_API __declspec(dllimport)
#endif

namespace LibSWBF2::API
{
	// be carefull in the futute!
	// there might be multiple chunks with the same name
	using namespace Chunks::MSH;
	using namespace Types;
	using namespace Logging;

	extern "C"
	{
		LIBSWBF2_API MSH* MSH_LoadFromFile(const char* path);
		LIBSWBF2_API MSH* MSH_Create();
		LIBSWBF2_API void MSH_Delete(MSH* ptr);

		LIBSWBF2_API void AddLogMessage(const char* message, const ELogType logType);
		LIBSWBF2_API void GetLogMessages(char* messages, const int32_t length, const ELogType logType);
		LIBSWBF2_API void GetLastLogMessages(char* messages, const int32_t length, const ELogType logType);
	}
}