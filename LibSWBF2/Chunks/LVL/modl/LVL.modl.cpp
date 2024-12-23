#include "pch.h"
#include "LVL.modl.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::modl
{
    void modl::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void modl::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void modl::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "NAME"_h)
            {
		p_Name = ReadChild<STR<"NAME"_m>>(stream);
            }
            else if (next == "NODE"_h)
            {
		p_Node = ReadChild<STR<"NODE"_m>>(stream);
            }
            else if (next == "INFO"_h)
            {
		p_Info = ReadChild<INFO>(stream);
            }
            else if (next == "segm"_h)
            {
		m_Segments.emplace_back(ReadChild<segm>(stream));
            }
            else
            {
                (void) ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string modl::ToString() const
    {
        return
            "NAME: " + p_Name->m_Text + "\n\n" +
            "INFO:\n" +
            p_Info->ToString() + "\n\n" +
            "Number of Segments: " + std::to_string(m_Segments.size());
    }
}
