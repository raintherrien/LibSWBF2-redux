#pragma once
#include "Chunks/GenericChunk.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct LIBSWBF2_API MINA : public GenericChunk // ANIM
	{

		std::vector<uint32_t> m_TransBitFlags;
		std::vector<uint32_t> m_AnimNameHashes;
		std::vector<uint16_t> m_AnimFrameCounts;
		std::vector<uint16_t> m_AnimBoneCounts;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "MINA"_m; }
	};
}
