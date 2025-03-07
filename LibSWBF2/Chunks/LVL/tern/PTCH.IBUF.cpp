#include "pch.h"
#include "PTCH.IBUF.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::terrain
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

        m_ElementCount = stream.ReadUInt32();
        for (uint32_t i = 0; i < m_ElementCount; ++i)
        {
            m_IndexBuffer.push_back(stream.ReadUInt16());
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string IBUF::ToString() const
    {
        std::string result = fmt::format(
            "Element Count: {}\n"
            "\n"
            "Buffer: [",
            m_ElementCount
        );

        for (uint32_t i = 0; i < m_IndexBuffer.size(); ++i)
        {
            result += std::to_string(m_IndexBuffer[i]) + ", ";
        }
        result.resize(result.size() - 2);
        result += "]";

        return result;
    }
}
