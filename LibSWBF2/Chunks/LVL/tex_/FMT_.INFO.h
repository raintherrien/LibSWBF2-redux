#pragma once
#include "Chunks/GenericChunk.h"
#include "DirectX/D3D9FORMAT.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::FMT
{
	struct LIBSWBF2_API INFO : public GenericChunk
	{
		D3DFORMAT m_Format;
		uint16_t m_Width;
		uint16_t m_Height;
		uint16_t m_Depth;
		uint16_t m_MipmapCount;
		uint32_t m_Type;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
