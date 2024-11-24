#include "pch.h"
#include "regn.h"
#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
    void regn::RefreshSize()
    {
        THROW("Not implemented!");
    }

    void regn::WriteToStream(FileWriter& stream)
    {
        THROW("Not implemented!");
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
                READ_CHILD(stream, p_Info);
            }
            else if (next == "PROP"_h)
            {
                READ_CHILD(stream, m_Props.emplace_back());
            }
            else 
            {
                READ_CHILD_GENERIC(stream);
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
