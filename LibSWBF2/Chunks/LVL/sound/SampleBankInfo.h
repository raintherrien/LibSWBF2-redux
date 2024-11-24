#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Enums.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::sound
{
	struct LIBSWBF2_API SampleBankInfo : public GenericChunk<"Info"_fnv>
	{
		FNVHash m_Name;
		ESoundFormat m_Format;
		uint32_t m_NumSamples;
		uint32_t m_NumChannels;
		uint32_t m_CombinedSoundSize;
		uint32_t m_Padding;

		std::vector<Types::SoundClip> m_SoundHeaders; //TODO: make a separate type for SoundHeaders in bnk/emo_


		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		std::string HeaderToString() const;
		std::string SampleInfoToString() const;
	};
}
