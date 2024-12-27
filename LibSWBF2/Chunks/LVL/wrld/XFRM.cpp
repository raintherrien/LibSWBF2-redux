#include "pch.h"
#include "XFRM.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
    void XFRM::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void XFRM::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void XFRM::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_RotationMatrix.ReadFromStream(stream);
        m_Position.ReadFromStream(stream);

        BaseChunk::EnsureEnd(stream);
    }

    std::string XFRM::ToString() const
    {
        return
            "Rotation:\n" + m_RotationMatrix.ToString() + "\n\n" +
            "Position: " + m_Position.ToString();
    }
}
