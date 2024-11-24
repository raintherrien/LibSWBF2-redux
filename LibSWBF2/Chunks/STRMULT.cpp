#include "pch.h"
#include "STRMULT.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks
{
    template<uint32_t Header>
    void STRMULT<Header>::RefreshSize()
    {
        THROW("Not implemented!");
    }

    template<uint32_t Header>
    void STRMULT<Header>::WriteToStream(FileWriter& stream)
    {
        THROW("Not implemented!");
    }

    template<uint32_t Header>
    void STRMULT<Header>::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        GenericChunk<Header>::Check(stream);

        m_Texts.clear();
        while (BaseChunk::PositionInChunk(stream.GetPosition()))
        {
		std::string next = stream.ReadString();
            if (!next.empty())
            {
                m_Texts.push_back(next);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }

    template<uint32_t Header>
    std::string STRMULT<Header>::ToString() const
    {
        std::string result;
        for (size_t i = 0; i < m_Texts.size(); ++i)
        {
            result += m_Texts[i] + std::string("\n");
        }
        return result.c_str();
    }
}

namespace LibSWBF2::Chunks
{
    template LIBSWBF2_API struct STRMULT<"NAME"_m>;
    template LIBSWBF2_API struct STRMULT<"PRNT"_m>;
    template LIBSWBF2_API struct STRMULT<"ANIM"_m>;
}
