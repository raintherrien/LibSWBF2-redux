#pragma once
#include "Chunks/GenericChunk.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct LIBSWBF2_API MINA : public GenericChunk<"MINA"_m> //ANIM
	{
	public:

		std::vector<uint32_t> m_TransBitFlags;
		std::vector<uint32_t> m_AnimNameHashes;
		std::vector<uint16_t> m_AnimFrameCounts;
		std::vector<uint16_t> m_AnimBoneCounts;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
