
#include "pch.h"
#include "StreamData.h"
#include "Stream.h"
#include "StreamInfo.h"
#include "InternalHelpers.h"
#include "FileReader.h"


namespace LibSWBF2::Chunks::LVL::sound
{
	void StreamData::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void StreamData::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void StreamData::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		Stream *soundStream = dynamic_cast<Stream*>(GetParent());
		if (soundStream == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Tried to read StreamData from a chunk other than Stream");
		}
		else 
		{
			std::shared_ptr<StreamInfo> info = soundStream -> p_Info;
			if (info == nullptr)
			{
				LIBSWBF2_LOG_ERROR("Tried to read StreamData, but parent Stream's Info chunk was missing!");
			}
			else 
			{
				for (int i = 0; i < info -> m_SoundHeaders.size(); i++)
				{
					Types::SoundClip& currClip = info -> m_SoundHeaders[i];

					size_t EndPos = stream.GetPosition() + currClip.m_DataLength + currClip.m_Padding;
					//if (!PositionInChunk(EndPos - 1))
					//{
						//LIBSWBF2_LOG_ERROR("Attempt to read Stream's SoundClip data would overshoot file...");
						//break;
					//}

					// If sound header has alias we skip it
					if (currClip.m_Alias != 0)
					{
						LIBSWBF2_LOG_WARN("Encountered aliased stream in file: {}, stream {:#x}, segment {:#x}", stream.GetFileName(), info -> m_Name, currClip.m_NameHash);
						continue;
					}

					currClip.m_DataPosition = stream.GetPosition();

					if (!PositionInChunk(currClip.m_DataPosition + currClip.m_DataLength - 1))
					{
						LIBSWBF2_LOG_WARN("Stream segment's data overruns chunk by {} bytes. (file: {}, stream: {:#x})", 
								(currClip.m_DataPosition + currClip.m_DataLength) - (GetDataPosition() + m_Size),
								stream.GetFileName(), info -> m_Name);
						continue;
					}
					else 
					{
						stream.SkipBytes(currClip.m_DataLength);
					}

					if (!PositionInChunk(stream.GetPosition() + currClip.m_Padding - 1))
					{
						LIBSWBF2_LOG_WARN("Stream segment's padding overruns chunk by {} bytes. (file: {}, stream: {:#x})", 
								(stream.GetPosition() + currClip.m_Padding) - (GetDataPosition() + m_Size),
								stream.GetFileName(), info -> m_Name);
						continue;
					}
					else 
					{
						stream.SkipBytes(currClip.m_Padding);
					}
				}
			}
		}

		BaseChunk::EnsureEnd(stream);
	}
}
