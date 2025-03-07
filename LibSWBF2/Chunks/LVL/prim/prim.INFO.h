#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::prim
{
	struct LIBSWBF2_API INFO_prim : public GenericChunk
	{
	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		std::string m_ModelName;
		uint16_t m_NumPrimitives;

		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
