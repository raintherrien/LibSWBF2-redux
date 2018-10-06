#include "stdafx.h"
#include "API.h"

namespace LibSWBF2::API
{
	// Logging //
	LIBSWBF2_API void LOG_SetCallbackMethod(function<void(LoggerEntry)>* Callback)
	{
		if (!Callback)
		{
			LOG("[API] Given Callback function was invalid!", ELogType::Warning);
			return;
		}

		Logger::GetInstance()->m_OnLogCallback = *Callback;
	}

	LIBSWBF2_API void LOG_Log(const char* message, const ELogType logType)
	{
		LOG("[API] " + string(message), logType);
	}


	// MSH //
	LIBSWBF2_API MSH* MSH_Create()
	{
		return new MSH();
	}

	LIBSWBF2_API void MSH_Delete(MSH* msh)
	{
		if (!msh)
		{
			LOG("[API] Given MSH pointer was NULL!", ELogType::Error);
			return;
		}

		delete msh;
	}

	LIBSWBF2_API void MSH_ReadFromFile(MSH* msh, const char* path)
	{
		if (!msh)
		{
			LOG("[API] Given MSH pointer was NULL!", ELogType::Error);
			return;
		}

		msh->ReadFromFile(string(path));
	}
}