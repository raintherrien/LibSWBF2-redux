#include "pch.h"
#include "PCHS.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::terrain
{
    void PCHS::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void PCHS::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void PCHS::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "PTCH"_h)
            {
                PTCH* patch;
                READ_CHILD(stream, patch);
                m_Patches.push_back(patch);
            }
            else
            {
                READ_CHILD_GENERIC(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string PCHS::ToString() const
    {
        return fmt::format("Num Patches: {}", m_Patches.size()).c_str();
    }
}
