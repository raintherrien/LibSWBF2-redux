#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::modl::segment
{
	struct LIBSWBF2_API INFO : public GenericChunk
	{
		ETopology m_Topology;
		uint32_t m_VertexCount;
		uint32_t m_PrimitiveCount;

		EModelPurpose GetPurpose();

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
