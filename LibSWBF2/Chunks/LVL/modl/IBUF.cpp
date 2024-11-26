#include "pch.h"
#include "IBUF.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::modl
{
    void IBUF::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void IBUF::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void IBUF::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_IndicesCount = stream.ReadUInt32();
        m_Indices.reserve(m_IndicesCount);
        for (uint32_t i = 0; i < m_IndicesCount; ++i)
        {
            m_Indices.push_back(stream.ReadUInt16());
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string IBUF::ToString() const
    {
        std::string result = fmt::format("Number of Inidces = {}\nIndices = [", m_IndicesCount);
        for (uint32_t i = 0; i < m_IndicesCount; ++i)
        {
            result += fmt::format("{}, ", m_Indices[i]);
        }

        result.resize(result.size() - 2);
        result += "]";
        return result;
    }
}
