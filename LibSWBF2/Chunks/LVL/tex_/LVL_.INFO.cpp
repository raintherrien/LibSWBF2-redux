#include "pch.h"
#include "tex_.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::LVL_texture
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

		m_MipLevel = stream.ReadUInt32();
		m_BodySize = stream.ReadUInt32();

		BaseChunk::EnsureEnd(stream);
	}

	std::string INFO::ToString() const
	{
		return fmt::format(
			"Mip Level:\t{}\n"
			"Body Size:\t{}\n",
			m_MipLevel, m_BodySize);
	}
}
