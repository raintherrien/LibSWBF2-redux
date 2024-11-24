#include "pch.h"
#include "StreamList.h"
#include "Stream.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::sound
{
	void StreamList::RefreshSize()
	{
		THROW("Not implemented!");
	}

	void StreamList::WriteToStream(FileWriter& stream)
	{
		THROW("Not implemented!");
	}

	void StreamList::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		while (ThereIsAnother(stream))
		{
			ChunkHeader next = stream.ReadChunkHeader(true);
			if (next == "Stream"_fnvh)
			{
				READ_CHILD(stream, m_Streams.emplace_back())
			}
			else 
			{
				stream.SkipBytes(4);
			}		
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string StreamList::ToString() const
	{
		std::string result = fmt::format(
			"Num streams: {0}",
			m_Streams.size()
		);

		return result;
	}
}
