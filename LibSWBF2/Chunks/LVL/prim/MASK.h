#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "../wrld/XFRM.h"
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::prim
{
	struct LIBSWBF2_API MASK : public GenericChunk
	{
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		uint32_t GetHeader() override { return "MASK"_m; }

		std::string ToString() const override;

		ECollisionMaskFlags m_MaskFlags; //solider, Ordnance, building, vehicle, etc...
	};
}
