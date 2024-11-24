#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/RawData.h"
#include "DATA.h"
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::config
{
	struct LIBSWBF2_API DATA_STRING : public DATA_CONFIG
	{
		uint32_t m_Tag;
		std::string m_String;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
