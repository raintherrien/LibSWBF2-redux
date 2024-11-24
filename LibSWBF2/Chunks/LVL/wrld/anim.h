#pragma once
#include "Chunks/GenericChunk.h"
#include "anim.INFO.h"
#include "WorldAnimKeyChunk.h"
#include <vector>

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct LIBSWBF2_API anim : public GenericChunk<"anim"_m>
	{
		anim_INFO* p_Info;

		std::vector<POSK *> m_PositionKeys;
		std::vector<ROTK *> m_RotationKeys;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};
}
