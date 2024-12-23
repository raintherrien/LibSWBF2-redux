#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STRMULT.h"
#include <vector>

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct anmg_INFO;

	struct LIBSWBF2_API NOHI : public GenericChunk
	{
		uint32_t GetHeader() override { return "NOHI"_m; }
	};

	struct LIBSWBF2_API anmg : public GenericChunk
	{
		std::shared_ptr<anmg_INFO> p_Info;

		std::vector<std::shared_ptr<STRMULT<"ANIM"_m>>> m_AnimObjectPairs;

		std::shared_ptr<NOHI> p_NoHierarchy; 

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		uint32_t GetHeader() override { return "anmg"_m; }
	};
}
