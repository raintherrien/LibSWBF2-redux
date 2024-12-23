#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::script
{
	struct LIBSWBF2_API INFO : public GenericChunk
	{
	public:
		uint32_t m_NumBodies;	// best guess so far

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
