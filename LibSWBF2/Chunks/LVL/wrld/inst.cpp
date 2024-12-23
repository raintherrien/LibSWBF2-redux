#include "pch.h"
#include "inst.h"
#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
    void inst::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void inst::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void inst::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_OverrideProperties.clear();
        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "INFO"_h)
            {
                p_Info = ReadChild<INFO>(stream);
            }
            else if (next == "PROP"_h)
            {
                m_OverrideProperties.emplace_back(ReadChild<PROP>(stream));
            }
            else
            {
                ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }


    std::string inst::ToString() const
    {
        std::string rep = fmt::format("Name: {}\nType: {}\nOverrides {} properties",
                                p_Info -> p_Name -> m_Text,
                                p_Info -> p_Type -> m_Text,
                                m_OverrideProperties.size());
        return rep;
    }
}
