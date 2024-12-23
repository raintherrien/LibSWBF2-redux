#include "pch.h"
#include "GenericClass.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::common
{
	void GenericClass::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void GenericClass::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void GenericClass::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);
		
		while (ThereIsAnother(stream))
		{
			ChunkHeader next = stream.ReadChunkHeader(true);
			if (next == "BASE"_h)
			{
				p_Base = ReadChild<STR<"BASE"_m>>(stream);
			}
			else if (next == "TYPE"_h)
			{
				p_Type = ReadChild<STR<"TYPE"_m>>(stream);
			}
			else if (next == "PROP"_h)
			{
				m_Properties.emplace_back(ReadChild<PROP>(stream));
			}
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string GenericClass::ToString() const
	{
		return fmt::format(
			"Base: {}\n\n"
			"Type: {}\n",
			p_Base->m_Text,
			p_Type->m_Text
		);
	}
}
