#pragma once
#include "Chunks/GenericChunk.h"
#include "anim.INFO.h"
#include "WorldAnimKeyChunk.h"
#include <vector>

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct LIBSWBF2_API anim : public GenericChunk
	{
		std::shared_ptr<anim_INFO> p_Info;

		std::vector<std::shared_ptr<POSK>> m_PositionKeys;
		std::vector<std::shared_ptr<ROTK>> m_RotationKeys;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		uint32_t GetHeader() override { return "anim"_m; }
	};
}
