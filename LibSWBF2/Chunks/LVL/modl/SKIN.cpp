#include "pch.h"
#include "SKIN.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::modl
{
    void SKIN::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void SKIN::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void SKIN::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_VertexCount = stream.ReadUInt32();
        m_Type = stream.ReadUInt32();

        if (m_Type == 1)
        {
            for (uint32_t i = 0; i < m_VertexCount; ++i)
            {
                m_BoneIndices.push_back(stream.ReadByte());
            }
        }
        else
        {
            LIBSWBF2_LOG_WARN("Ignore yet unsupported SKIN type {}!", m_Type);
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string SKIN::ToString() const
    {
        std::string result = fmt::format("Number of Vertices / Bone Indices = {}\nBone Indices = [", m_VertexCount);
        for (uint32_t i = 0; i < m_VertexCount && i < m_BoneIndices.size(); ++i)
        {
            result += fmt::format("{}, ", m_BoneIndices[i]);
        }

        result.resize(result.size() - 2);
        result += "]";
        return result;
    }
}
