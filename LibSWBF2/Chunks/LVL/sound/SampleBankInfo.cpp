#include "pch.h"

#include "SampleBankInfo.h"
#include "SampleBank.h"
#include "InternalHelpers.h"
#include "FileReader.h"


namespace LibSWBF2::Chunks::LVL::sound
{
	void SampleBankInfo::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void SampleBankInfo::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void SampleBankInfo::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		bool breakOut = false;

		while (ThereIsAnother(stream))
		{
			FNVHash next = (FNVHash) stream.ReadUInt32();

			switch (next)
			{
				case "Name"_fnv:
					m_Name = (FNVHash) stream.ReadUInt32();
					break;

				case "Format"_fnv:
					m_Format = (ESoundFormat) stream.ReadUInt32(); 
					break;

				case "NumChannels"_fnv:
					m_NumChannels = stream.ReadUInt32();
					break;

				case "NumSamples"_fnv:
					m_NumSamples = stream.ReadUInt32();
					break;

				case "Size"_fnv:
					m_CombinedSoundSize = stream.ReadUInt32(); 
					break;

				case "SampleInfo"_fnv:
					next = (FNVHash) stream.ReadUInt32();
					while (next == "Sample"_fnv)
					{
						m_SoundHeaders.emplace_back().ReadHeaderFromStream(stream);
						next = (FNVHash) stream.ReadUInt32(); 
					}
					stream.SetPosition(stream.GetPosition() - 4);
					break;	

				case "Padding"_fnv:
					m_Padding = stream.ReadUInt32(); 
					breakOut = true;
					break;		

				default:
					break;
			}

			if (breakOut)
			{ 
				break;
			}
		}

		BaseChunk::EnsureEnd(stream);
	}


	std::string SampleBankInfo::HeaderToString() const
	{
		return fmt::format(
			"Name: 0x{0:x}\n"
			"Format: {1}\n"
			"Num Samples: {2}\n"
			"Combined Sound Size: {3}\n"
			"Padding: {4}\n",
			m_Name,
			SoundFormatToString(m_Format),
			m_NumSamples,
			m_CombinedSoundSize,
			m_Padding
		);
	}

	std::string SampleBankInfo::SampleInfoToString() const
	{
		std::string soundsStr;
		for (int i = 0; i < m_SoundHeaders.size(); i++)
		{
			soundsStr = soundsStr + fmt::format("\n{}:\n", i);
			soundsStr = soundsStr + m_SoundHeaders[i].ToString();
		}
		return soundsStr;
	}

	std::string SampleBankInfo::ToString() const
	{
		return HeaderToString() + "\nSample headers: \n" + SampleInfoToString();
	}
}
