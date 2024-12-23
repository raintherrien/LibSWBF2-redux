#include "pch.h"
#include "regn.h"
#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
    void regn::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void regn::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void regn::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "INFO"_h)
            {
                p_Info = ReadChild<INFO>(stream);
            }
            else if (next == "PROP"_h)
            {
                m_Props.emplace_back(ReadChild<PROP>(stream));
            }
            else 
            {
                ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }


    std::string regn::ToString() const
    {
        std::string rep = fmt::format("Name: {}\nType: {}\n",
                                p_Info -> p_Name -> m_Text,
                                p_Info -> p_Type -> m_Text);
        return rep;
    }
}
