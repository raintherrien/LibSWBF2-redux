#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Chunks::LVL
{
	using Types::SoundBankHeader;

	struct LIBSWBF2_API lvl_ : public GenericChunk
	{
		uint32_t m_NameHash = 0;
		uint32_t m_SizeLeft = 0;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		bool TryLookupName(std::string& result);
		std::string ToString() const override;
		uint32_t GetHeader() override { return "lvl_"_m; }
	};
}
