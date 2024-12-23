#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::modl
{
	// Index Buffer
	struct LIBSWBF2_API IBUF : public GenericChunk
	{
		uint32_t m_IndicesCount;
		std::vector<uint16_t> m_Indices;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	
		uint32_t GetHeader() override { return "IBUF"_m; }
	};
}
