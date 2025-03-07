#include "pch.h"
#include "Sound.h"
#include "InternalHelpers.h"
#include "Audio/SoundDecoder.h"
#include "Audio/IMAADPCMDecoder.h"


namespace LibSWBF2::Wrappers
{
	bool Sound::FromSoundClip(SoundClip* soundClip, Sound& out)
	{
		if (soundClip == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given soundClip was NULL!");
			return false;
		}

		out.p_SoundClip = soundClip;

		return true;
	}

	FNVHash Sound::GetHashedName() const
	{
		return p_SoundClip->m_NameHash;
	}

	ESoundFormat Sound::GetFormat() const
	{
		return m_Format;
	}

	uint32_t Sound::GetNumChannels() const
	{
		return m_NumChannels;
	}

	uint32_t Sound::GetSampleRate() const
	{
		return p_SoundClip->m_SampleRate;
	}

	uint32_t Sound::GetNumSamples() const
	{
		return p_SoundClip->m_SampleCount;
	}

	uint32_t Sound::GetAlias() const
	{
		return p_SoundClip->m_Alias;
	}

	const uint8_t* Sound::GetDataPtr() const
	{
		return p_SoundClip->GetSampleData();
	}

	size_t Sound::GetDataLength() const
	{
		return p_SoundClip->m_DataLength;
	}


	bool Sound::FillDataBuffer(ESoundFormat destFormat, int16_t* bufferToFill) const
	{
		if (!HasData()) return false;

		if (destFormat == ESoundFormat::PCM16 && m_Format == ESoundFormat::PCM16)
		{
			memcpy((void*)bufferToFill, (void*)p_SoundClip->GetSampleData(), p_SoundClip->m_DataLength);
			return true;
		}
		else if (destFormat == ESoundFormat::PCM16 && m_Format == ESoundFormat::IMAADPCM)
		{
			IMAADPCMDecoder dec(GetNumChannels(), 4);
			size_t numBytesRead;
			dec.DecodeAndFillPCM16((void *) GetDataPtr(), GetDataLength(), bufferToFill, GetNumSamples(), numBytesRead);
			return true;
		}
		else 
		{
			return false;
		}
	}


	size_t Sound::GetDataPosition() const 
	{
		return p_SoundClip->m_DataPosition;
	}






	bool Sound::GetData(uint32_t& sampleRate, uint32_t& sampleCount, uint8_t& blockAlign, const uint8_t*& data) const
	{
		if (p_SoundClip == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Encapsuled SoundClip is NULL!");
			return false;
		}

		sampleRate = p_SoundClip->m_SampleRate;
		sampleCount = p_SoundClip->m_SampleCount;
		blockAlign = p_SoundClip->GetBytesPerSample();
		data = p_SoundClip->GetSampleData();
		return data != nullptr;
	}

	bool Sound::HasData() const
	{
		return p_SoundClip -> GetSampleData() != nullptr;
	}
}
