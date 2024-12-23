#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct LIBSWBF2_API anim_INFO : public GenericChunk
	{
		std::string m_Name;
		
		float m_RunTime;
		uint8_t m_Looping;
		uint8_t m_LocalTranslation;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		
		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
