#include "pch.h"
#include "skel.INFO.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::skel
{
    void INFO::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void INFO::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void INFO::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_ModelName = stream.ReadString();
        m_BoneCount = stream.ReadUInt32();

        BaseChunk::EnsureEnd(stream);
    }

    std::string INFO::ToString() const
    {
        return fmt::format(
            "Model Name: {}\n"
            "Bone Count: {}\n",
            m_ModelName,
            m_BoneCount
        ).c_str();
    }
}
