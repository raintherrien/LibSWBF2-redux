#pragma once
#include "Chunks/GenericChunk.h"
#include "PTCH.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::terrain
{
	struct LIBSWBF2_API PCHS : public GenericChunk
	{
		std::vector<std::shared_ptr<PTCH>> m_Patches;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "PCHS"_m; }
	};
}
