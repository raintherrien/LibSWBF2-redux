#pragma once
#include "Chunks/GenericChunk.h"
#include "DirectX/D3D9FORMAT.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::texture
{
	struct LIBSWBF2_API INFO : public GenericChunk
	{
		uint32_t m_FormatCount;
		std::vector<D3DFORMAT> m_Formats;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
