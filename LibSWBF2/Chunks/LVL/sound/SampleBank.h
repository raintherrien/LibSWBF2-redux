#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::sound
{
	struct SampleBankInfo;
	struct SampleBankData;

	struct LIBSWBF2_API SampleBank : public GenericChunk
	{
		std::shared_ptr<SampleBankInfo> p_Info;
		std::shared_ptr<SampleBankData> p_Data;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "SampleBank"_fnv; }
	};
}
