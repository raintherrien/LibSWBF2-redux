#pragma once
#include "Chunks/GenericChunk.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct LIBSWBF2_API anmh_INFO : public GenericChunk
	{
		uint8_t m_NumStrings;

		std::string m_RootName;
		std::vector<std::string> m_ChildNames;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		
		std::string ToString() const override;

		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
