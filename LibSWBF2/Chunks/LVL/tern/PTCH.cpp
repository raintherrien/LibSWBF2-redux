#include "pch.h"
#include "PTCH.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::terrain
{
    void PTCH::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void PTCH::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void PTCH::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        // IBUFs don't have any flag of some sort. Apparently the only way
        // to distinguish a Gemoetry IBUF from a TextureExtra IBUF is the
        // chunk order. 
        // The Geometry IBUF chunk comes right after the Geometry VBUF,
        // same for TextureExtra IBUF
        static ETerrainBufferType lastVBUFType; // XXX Race condition

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "VBUF"_h)
            {
                std::shared_ptr<VBUF> bufferChunk = ReadChild<VBUF>(stream);
                
                switch (bufferChunk->m_BufferType)
                {
                    case ETerrainBufferType::Texture:
                        m_TextureBuffer = bufferChunk;
                        break;
                    case ETerrainBufferType::Geometry:
                        m_GeometryBuffer = bufferChunk;
                        break;
                    case ETerrainBufferType::TextureExtra:
                        m_TextureExtraBuffer = bufferChunk;
                        break;
                    default:
                        LIBSWBF2_LOG_WARN("Unknown VBUF chunk with type: {}", TerrainBufferTypeToString(bufferChunk->m_BufferType));
                        break;
                }
                lastVBUFType = bufferChunk->m_BufferType;
            }
            else if (next == "IBUF"_h)
            {
                std::shared_ptr<IBUF> bufferChunk = ReadChild<IBUF>(stream);

                switch (lastVBUFType)
                {
                    case ETerrainBufferType::Texture:
                        LIBSWBF2_LOG_WARN("Unexpected IBUF found right after Texture VBUF!");
                        break;
                    case ETerrainBufferType::Geometry:
                        m_GeometryIndexBuffer = bufferChunk;
                        break;
                    case ETerrainBufferType::TextureExtra:
                        m_TextureExtraIndexBuffer = bufferChunk;
                        break;
                    default:
                        LIBSWBF2_LOG_WARN("Unknown last VBUF chunk with type: {}", TerrainBufferTypeToString(lastVBUFType));
                        break;
                }
            }
            else if (next == "INFO"_h)
            {
		p_PatchInfo = ReadChild<PTCH_INFO>(stream);
            }
            else
            {
                ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string PTCH::ToString() const
    {
        return fmt::format(
            "Has Texture Vertex Buffer: {}\n"
            "Has Geometry Vertex Buffer: {}\n"
            "Has Texture Extra Vertex Buffer: {}\n"
            "\n"
            "Has Geometry IndexBuffer: {}\n"
            "Has Texture Extra IndexBuffer: {}\n",
            m_TextureBuffer != nullptr ? "yes" : "no",
            m_GeometryBuffer != nullptr ? "yes" : "no",
            m_TextureExtraBuffer != nullptr ? "yes" : "no",
            m_GeometryIndexBuffer != nullptr ? "yes" : "no",
            m_TextureExtraIndexBuffer != nullptr ? "yes" : "no"
        );
    }
}
