#include "pch.h"
#include "SIZE.h"
#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
    void SIZE::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void SIZE::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void SIZE::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_Dimensions.ReadFromStream(stream);

        BaseChunk::EnsureEnd(stream);
    }

    std::string SIZE::ToString() const
    {
        return m_Dimensions.ToString();
    }
}
