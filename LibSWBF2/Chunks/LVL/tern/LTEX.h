#pragma once
#include "Chunks/GenericChunk.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::terrain
{
	struct LIBSWBF2_API LTEX : public GenericChunk
	{
		std::vector<std::string> m_LayerTextures;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "LTEX"_m; }
	};
}
