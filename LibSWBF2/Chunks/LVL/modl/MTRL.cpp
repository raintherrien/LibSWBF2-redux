#include "pch.h"
#include "MTRL.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::modl
{
    void MTRL::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void MTRL::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void MTRL::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_Flags = (EMaterialFlags)stream.ReadUInt32();
        m_DiffuseColor.ReadFromStream(stream);
        m_SpecularColor.ReadFromStream(stream);
        m_SpecularExponent = stream.ReadUInt32();
        m_Parameters[0] = stream.ReadUInt32();
        m_Parameters[1] = stream.ReadUInt32();
        m_AttachedLight = stream.ReadString();

        BaseChunk::EnsureEnd(stream);
    }

    std::string MTRL::ToString() const
    {
        return
            "Flags = " + MaterialFlagsToString(m_Flags) + "\n" +
            "Diffuse Color = " + m_DiffuseColor.ToString() + "\n" +
            "Specular Color = " + m_SpecularColor.ToString() + "\n" +
            "Specular Exponent = " + std::to_string(m_SpecularExponent) + "\n" +
            "Attached Light = " + m_AttachedLight;
    }
}
