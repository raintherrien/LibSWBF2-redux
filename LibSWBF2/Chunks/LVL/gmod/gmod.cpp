#include "pch.h"
#include "gmod.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::gmod
{
    void gmod::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void gmod::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void gmod::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);
        BaseChunk::EnsureEnd(stream);
    }
}
