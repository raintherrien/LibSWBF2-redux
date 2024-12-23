#pragma once
#include "Chunks/GenericChunk.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks
{
	template<uint32_t Header>
	struct LIBSWBF2_API STRMULT : public GenericChunk
	{
	public:
		std::vector<std::string> m_Texts;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return Header; }
	};
}
