#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/LocalizeEntry.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::Localization
{
	using Types::LocalizeEntry;

	struct LIBSWBF2_API BODY : public GenericChunk
	{
		std::vector<LocalizeEntry> m_LocalizeEntries;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "BODY"_m; }
	};
}
