#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::sound
{
	struct SampleBankInfo;
	struct SampleBankData;

	struct LIBSWBF2_API SampleBank : public GenericChunk<"SampleBank"_fnv>
	{
		SampleBankInfo *p_Info;
		SampleBankData *p_Data;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
