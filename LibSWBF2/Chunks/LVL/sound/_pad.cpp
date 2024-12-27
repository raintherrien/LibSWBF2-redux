#include "pch.h"
#include "_pad.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::sound
{
	void _pad::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void _pad::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void _pad::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		// for some reason, they don't write any data size
		// TODO: if WriteToStream is ever implemented, make
		// sure to write also 0 as size back!

		m_Unknown1 = stream.ReadUInt32();

		// Is always the size of the following Stream chunk
		m_SizeNextStreamChunk = stream.ReadUInt32();

		if (m_Unknown1 != 1538883361)
		{
			LIBSWBF2_LOG_WARN("Found unusual m_Unknown1 value in a _pad chunk: {}", m_Unknown1);
		}

		m_Size = 8;

		BaseChunk::EnsureEnd(stream);
	}

	std::string _pad::ToString() const
	{
		return fmt::format(
			"Unknown 1: {}\n"
			"Size of next stream chunk: {}",
			m_Unknown1,
			m_SizeNextStreamChunk
		);
	}
}
