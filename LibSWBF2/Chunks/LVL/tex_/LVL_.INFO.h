#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::LVL_texture
{
	struct LIBSWBF2_API INFO : public GenericChunk
	{
		uint32_t m_MipLevel;
		uint32_t m_BodySize; // why did they put that here...

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
