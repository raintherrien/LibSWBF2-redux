#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::sound
{
	struct LIBSWBF2_API _pad : public GenericChunk
	{
		uint32_t m_Unknown1;
		uint32_t m_SizeNextStreamChunk;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "_pad"_m; }
	};
}
