#include "pch.h"

#include "StreamInfo.h"
#include "Stream.h"
#include "InternalHelpers.h"
#include "FileReader.h"


namespace LibSWBF2::Chunks::LVL::sound
{
	void StreamInfo::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void StreamInfo::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void StreamInfo::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

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

				case "ChannelInterleave"_fnv:
					m_ChannelInterleave = stream.ReadUInt32();
					break;

				case "NumSegments"_fnv:
					m_NumSegments = stream.ReadUInt32();
					break;

				case "NumSubstreams"_fnv:
					m_NumSubstreams = stream.ReadUInt32();
					break;

				case "SubstreamInterleave"_fnv:
					m_SubstreamInterleave = stream.ReadUInt32();
					break;

				case "Size"_fnv:
					m_CombinedSoundSize = stream.ReadUInt32(); 
					break;

				case "SegmentInfo"_fnv:
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
					stream.SkipBytes(m_Padding - 4);
					break;		

				default:
					break;
			}
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string StreamInfo::HeaderToString() const
	{
		return fmt::format(
			"Name: 0x{0:x}\n"
			"Format: {1}\n"
			"Num Channels: {2}\n"
			"Channel Interleave: {3}\n"
			"Num Segments: {4}\n"
			"Num Substreams: {5}\n"
			"Substream Interleave: {6}\n"
			"Combined Sound Size: {7}\n"
			"Padding: {8}\n",
			m_Name,
			SoundFormatToString(m_Format),
			m_NumChannels,
			m_ChannelInterleave,
			m_NumSegments,
			m_NumSubstreams,
			m_SubstreamInterleave,
			m_CombinedSoundSize,
			m_Padding
		);
	}

	std::string StreamInfo::SegmentInfoToString() const
	{
		std::string soundsStr;
		for (int i = 0; i < m_SoundHeaders.size(); i++)
		{
			soundsStr = soundsStr + fmt::format("\n{}:\n", i);
			soundsStr = soundsStr + m_SoundHeaders[i].ToString();
		}
		return soundsStr;
	}

	std::string StreamInfo::ToString() const
	{
		return HeaderToString() + "\nSegment headers: \n" + SegmentInfoToString();
	}
}
