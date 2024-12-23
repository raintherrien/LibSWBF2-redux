#include "pch.h"
#include "wrld.INFO.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
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

        p_Type = ReadChild<STR<"TYPE"_m>>(stream);
        p_Name = ReadChild<STR<"NAME"_m>>(stream);
        p_XFRM = ReadChild<XFRM>(stream);

        if (ThereIsAnother(stream))
        {
            p_SIZE = ReadChild<SIZE>(stream);
        }

        BaseChunk::EnsureEnd(stream);
    }
}
