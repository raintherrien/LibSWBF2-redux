#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/WorldAnimationKey.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::wrld
{
	template<uint32_t Header>
	struct LIBSWBF2_API WorldAnimKeyChunk : public GenericChunk
	{
		Types::WorldAnimationKey m_Key;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return Header; }
	};

	struct LIBSWBF2_API POSK : public WorldAnimKeyChunk<"POSK"_m> {};
	struct LIBSWBF2_API ROTK : public WorldAnimKeyChunk<"ROTK"_m> {};
}
