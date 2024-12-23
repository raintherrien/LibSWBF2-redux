#pragma once
#include "Chunks/GenericChunk.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::sound
{
	struct SampleBank;

	struct LIBSWBF2_API SoundBankList : public GenericChunk
	{
		std::vector<std::shared_ptr<SampleBank>> m_SampleBanks;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "SoundBankList"_fnv; }
	};
}
