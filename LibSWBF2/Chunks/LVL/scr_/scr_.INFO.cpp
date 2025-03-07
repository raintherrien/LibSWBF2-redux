#include "pch.h"
#include "scr_.INFO.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::script
{
	void INFO::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void INFO::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void INFO::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		m_NumBodies = stream.ReadUInt32();

		BaseChunk::EnsureEnd(stream);
	}

	std::string INFO::ToString() const
	{
		return fmt::format(
			"Num Bodies: {}",
			m_NumBodies
		);
	}
}
