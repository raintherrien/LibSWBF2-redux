#include "pch.h"
#include "BMAP.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::modl
{
    void BMAP::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BMAP::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BMAP::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_IndexCount = stream.ReadUInt32();
        for (uint32_t i = 0; i < m_IndexCount; ++i)
        {
            m_IndexMap.push_back(stream.ReadByte());
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string BMAP::ToString() const
    {
        std::string result = fmt::format("Index Map Count = [", m_IndexCount);
        for (uint32_t i = 0; i < m_IndexCount; ++i)
        {
            result += fmt::format("{}, ", m_IndexMap[i]);
        }

        result.resize(result.size() - 2);
        result += "]";
        return result;
    }
}
