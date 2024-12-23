#include "pch.h"
#include "tern.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include <fmt/format.h>

namespace LibSWBF2::Chunks::LVL::terrain
{
    void tern::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void tern::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void tern::ReadFromStream(FileReader& stream)
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
            else if (next == "DTLX"_h)
            {
		p_DetailTexture = ReadChild<STR<"DTLX"_m>>(stream);
            }
            else if (next == "INFO"_h)
            {
		p_Info = ReadChild<INFO>(stream);
            }
            else if (next == "LTEX"_h)
            {
		p_LayerTextures = ReadChild<LTEX>(stream);
            }
            else if (next == "DTEX"_h)
            {
		p_LayerDetailTextures = ReadChild<DTEX>(stream);
            }
            else if (next == "PCHS"_h)
            {
                p_Patches = ReadChild<PCHS>(stream);
            }
            else
            {
                (void) ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string tern::ToString() const
    {
        return fmt::format("Name: {}", p_Name->m_Text);
    }
}
