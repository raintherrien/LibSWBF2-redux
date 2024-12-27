#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Enums.h"
#include "Types/TerrainBufferEntry.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::terrain
{
	struct LIBSWBF2_API IBUF : public GenericChunk<"IBUF"_m>
	{
	public:
		uint32_t m_ElementCount;
		std::vector<uint16_t> m_IndexBuffer;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
