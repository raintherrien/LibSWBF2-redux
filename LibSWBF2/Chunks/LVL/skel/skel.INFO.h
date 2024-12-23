#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Enums.h"
#include "Types/Vector3.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::skel
{
	struct LIBSWBF2_API INFO : public GenericChunk
	{
		std::string m_ModelName;
		uint32_t m_BoneCount;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
