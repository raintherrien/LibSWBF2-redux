#include "pch.h"
#include "TNAM.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::modl
{
    void TNAM::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void TNAM::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void TNAM::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_Index = stream.ReadUInt32();
        m_Name = stream.ReadString();

        BaseChunk::EnsureEnd(stream);
    }

    std::string TNAM::ToString() const
    {
        return 
            "Index = " + std::string(std::to_string(m_Index)) + "\n" +
            "Name = " + m_Name;
    }
}
