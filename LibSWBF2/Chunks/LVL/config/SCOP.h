#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/LVL/config/DATA.h"

namespace LibSWBF2::Chunks::LVL::config
{
	struct LIBSWBF2_API SCOP : public GenericChunk
	{
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		uint32_t GetHeader() override { return "SCOP"_m; }
	};
}
