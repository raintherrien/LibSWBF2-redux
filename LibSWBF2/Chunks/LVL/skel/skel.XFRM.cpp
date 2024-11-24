#include "pch.h"
#include "skel.XFRM.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "skel.h"

namespace LibSWBF2::Chunks::LVL::skel
{
    void XFRM::RefreshSize()
    {
        THROW("Not implemented!");
    }

    void XFRM::WriteToStream(FileWriter& stream)
    {
        THROW("Not implemented!");
    }

    void XFRM::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        skel* parent = dynamic_cast<skel*>(GetParent());
        if (parent == nullptr)
        {
            THROW("Parent is NOT a skel chunk!");
        }

        m_BoneRotations.clear();
        m_BonePositions.clear();
        for (uint32_t i = 0; i < parent->p_Info->m_BoneCount; ++i)
        {
            m_BoneRotations.emplace_back().ReadFromStream(stream);
            m_BonePositions.emplace_back().ReadFromStream(stream);
        }

        if (m_BoneRotations.size() != m_BonePositions.size())
        {
            THROW("Number of Bone Rotations {} does not match up with number of Bone Positions {}!", m_BoneRotations.size(), m_BonePositions.size());
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string XFRM::ToString() const
    {
        if (m_BoneRotations.size() != m_BonePositions.size())
        {
            THROW("Number of Bone Rotations {} does not match up with number of Bone Positions {}!", m_BoneRotations.size(), m_BonePositions.size());
        }

        std::string result;
        for (size_t i = 0; i < m_BoneRotations.size(); ++i)
        {
            result += fmt::format(
                "Rotation: \n{}\n"
                "Position: {}\n\n",
                m_BoneRotations[i].ToString(),
                m_BonePositions[i].ToString()
            );
        }
        return result;
    }
}
