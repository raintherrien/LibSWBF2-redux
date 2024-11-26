#include "pch.h"
#include "anmg.h"
#include "anmg.INFO.h"
#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"


namespace LibSWBF2::Chunks::LVL::wrld
{
    void anmg::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anmg::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anmg::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

		while (ThereIsAnother(stream))
		{
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "INFO"_h)
            {
                READ_CHILD(stream, p_Info);
            }
            else if (next == "ANIM"_h)
            {
                READ_CHILD(stream, m_AnimObjectPairs.emplace_back());
            }
            else if (next == "NOHI"_h)
            {
                READ_CHILD(stream, p_NoHierarchy);
            }
            else
            {
                READ_CHILD_GENERIC(stream);
            }
		}

        BaseChunk::EnsureEnd(stream);
    }
}
