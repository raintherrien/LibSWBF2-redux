#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Enums.h"
#include "Types/TerrainBufferEntry.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::terrain
{
	struct LIBSWBF2_API VBUF : public GenericChunk
	{
	public:
		uint32_t m_ElementCount;
		uint32_t m_ElementSize;
		ETerrainBufferType m_BufferType;
		std::vector<Types::TerrainBufferEntry> m_TerrainBuffer;

		std::vector<uint8_t> m_BlendMapData;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "VBUF"_m; }
	};
}
