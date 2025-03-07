#include "pch.h"
#include "LEKS.h"
#include "TNOJ.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::animation
{
	void LEKS::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void LEKS::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void LEKS::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);

			if (next == "TNOJ"_h)
			{
				m_Joints.emplace_back(ReadChild<TNOJ>(stream));
			}
			else 
			{
				break;
			}
        }

		BaseChunk::EnsureEnd(stream);
	}


	std::string LEKS::ToString() const
	{
		return fmt::format(
			"Animation skeleton with {} joints\n",
			m_Joints.size()
			);
	}
}
