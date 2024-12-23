#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Enums.h"
#include "Types/TerrainBufferEntry.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::terrain
{
	struct LIBSWBF2_API PTCH_INFO : public GenericChunk
	{

		std::vector<uint32_t> m_TextureSlotsUsed;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
