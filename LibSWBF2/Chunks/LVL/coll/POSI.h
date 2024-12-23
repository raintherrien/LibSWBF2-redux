#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::coll
{
	struct LIBSWBF2_API POSI : public GenericChunk
	{
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		uint32_t GetHeader() override { return "POSI"_m; }

		std::string ToString() const override;

		std::vector<Vector3> m_Verts;
	};
}
