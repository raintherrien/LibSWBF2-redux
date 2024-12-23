#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "Types/Enums.h"
#include "Types/Vector3.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::modl
{
	struct LIBSWBF2_API INFO : public GenericChunk
	{
		uint32_t m_Unknown[5];
		Vector3 m_VertexBox[2];
		Vector3 m_VisibilityBox[2];
		uint32_t m_FaceCount;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
