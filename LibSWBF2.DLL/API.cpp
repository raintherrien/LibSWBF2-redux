#include "stdafx.h"
#include "API.h"

namespace LibSWBF2::API
{
#define CheckPtr(msh, ret) if (msh == nullptr) { LOG("[API] Given Pointer was NULL!", ELogType::Error); return ret; }

	// Logging //
	void LOG_SetCallbackMethod(const LogCallback Callback)
	{
		CheckPtr(Callback,)
		Logger::SetLogCallback(Callback);
	}

	void LOG_SetLogfileLevel(ELogType LogfileLevel)
	{
		Logger::SetLogfileLevel(LogfileLevel);
	}


	// MSH //
	MSH* MSH_Create()
	{
		return MSH::Create();
	}

	bool MSH_Delete(MSH* msh)
	{
		CheckPtr(msh, false)
		MSH::Destroy(msh);
		return true;
	}

	bool MSH_ReadFromFile(MSH* msh, const char* path)
	{
		CheckPtr(msh, false)
		return msh->ReadFromFile(string(path));
	}

	bool MSH_WriteToFile(MSH* msh, const char* path)
	{
		CheckPtr(msh, false)
		return msh->WriteToFile(string(path));
	}

	void STRP_CalcPolygons(STRP* strp)
	{
		CheckPtr(strp,)
		strp->CalcPolygons();
	}

	EModelPurpose MODL_GetPurpose(MODL* modl)
	{
		CheckPtr(modl, EModelPurpose::Miscellaneous)
		return modl->GetPurpose();
	}

	CRCChecksum CalcLowerCRC(const char* str)
	{
		CheckPtr(str, 0)
		return LibSWBF2::CRC::CalcLowerCRC(str);
	}

	// Tools
	Level* Level_FromFile(const char* path)
	{
		return Level::FromFile(path);
	}

	void Level_Destroy(Level* level)
	{
		Level::Destroy(level);
	}
}