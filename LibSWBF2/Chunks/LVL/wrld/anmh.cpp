#include "pch.h"
#include "anmh.h"
#include "anmh.INFO.h"
#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
    void anmh::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anmh::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anmh::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "INFO"_h)
            {
		p_Info = ReadChild<anmh_INFO>(stream);
            }
            else
            {
                ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }
}
