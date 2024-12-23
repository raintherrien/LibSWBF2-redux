#pragma once
#include "GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks
{
	template<uint32_t Header>
	struct LIBSWBF2_API STR : public GenericChunk
	{
		std::string m_Text;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return Header; }

	private:
		inline bool IsValidStringChar(char c);
	};
}
