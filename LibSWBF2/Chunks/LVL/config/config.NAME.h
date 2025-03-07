#pragma once
#include "Chunks/GenericChunk.h"
#include "Hashing.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::config
{
	struct LIBSWBF2_API config_NAME : public GenericChunk
	{
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		FNVHash m_Name;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "NAME"_m; }
	};
}
