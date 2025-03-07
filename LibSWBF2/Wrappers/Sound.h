#pragma once
#include "Types/SoundClip.h"
#include "Types/Enums.h"

namespace LibSWBF2::Wrappers
{
	using LibSWBF2::Types::SoundClip;

	class Level;
	class SoundBank;
	class SoundStream;

	class LIBSWBF2_API Sound
	{
	private:
		friend Level;
		friend SoundBank;
		friend SoundStream;

	private:

		// This info is in the SampleBank/Stream's INFO_fnv chunk
		uint32_t m_NumChannels = 1;
		ESoundFormat m_Format = ESoundFormat::PCM16;

		SoundClip* p_SoundClip;

	public:
		Sound() = default;
		~Sound() = default;

		static bool FromSoundClip(SoundClip* soundClip, Sound& out);

		bool HasData() const;

		FNVHash GetHashedName() const;

		ESoundFormat GetFormat() const;
		uint32_t GetNumChannels() const;
		uint32_t GetSampleRate() const;
		uint32_t GetNumSamples() const;
		uint32_t GetAlias() const;


		bool FillDataBuffer(ESoundFormat format, int16_t* bufferToFill) const;

		const uint8_t* GetDataPtr() const;
		size_t GetDataLength() const;

		size_t GetDataPosition() const;


		bool GetData(uint32_t& sampleRate, uint32_t& sampleCount, uint8_t& blockAlign, const uint8_t*& data) const;
	};
}
