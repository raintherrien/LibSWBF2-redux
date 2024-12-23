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
                p_Info = ReadChild<anmg_INFO>(stream);
            }
            else if (next == "ANIM"_h)
            {
                m_AnimObjectPairs.emplace_back(ReadChild<STRMULT<"ANIM"_m>>(stream));
            }
            else if (next == "NOHI"_h)
            {
                p_NoHierarchy = ReadChild<NOHI>(stream);
            }
            else
            {
                ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }
}
