#include "pch.h"
#include "modl.segm.h"
#include "InternalHelpers.h"
#include "FileReader.h"

#include "MTRL.h"
#include "TNAM.h"
#include "IBUF.h"
#include "SKIN.h"
#include "BMAP.h"
#include "modl.VBUF.h"

namespace LibSWBF2::Chunks::LVL::modl
{
    void segm::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void segm::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void segm::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "INFO"_h)
            {
                p_Info = ReadChild<segment::INFO>(stream);
            }
            else if (next == "MTRL"_h)
            {
                p_Material = ReadChild<MTRL>(stream);
            }
            else if (next == "RTYP"_h)
            {
                p_RenderType = ReadChild<STR<"RTYP"_m>>(stream);
            }
            else if (next == "IBUF"_h)
            {
                p_IndexBuffer = ReadChild<IBUF>(stream);
            }
            else if (next == "BNAM"_h)
            {
                p_Parent = ReadChild<STR<"BNAM"_m>>(stream);
            }
            else if (next == "SKIN"_h)
            {
                p_Skin = ReadChild<SKIN>(stream);
            }
            else if (next == "BMAP"_h)
            {
                p_BoneMap = ReadChild<BMAP>(stream);
            }
            else if (next == "VBUF"_h)
            {
                m_VertexBuffers.emplace_back(ReadChild<VBUF>(stream));
            }
            else if (next == "TNAM"_h)
            {
                m_Textures.emplace_back(ReadChild<TNAM>(stream));
            }
            else if (next == "MNAM"_h)
            {
                p_Tag = ReadChild<STR<"MNAM"_m>>(stream);
            }
            else
            {
                (void) ReadChild<GenericChunk>(stream);
            }
        }

        if (p_Skin != nullptr && p_BoneMap == nullptr)
        {
            // does this ever exist?
            LIBSWBF2_LOG_WARN("Found model segment (segm) with SKIN, but without BMAP! Chunk pos: {:#x}", GetPosition());
        }

        BaseChunk::EnsureEnd(stream);
    }
}
