#include "pch.h"
#include "PROP.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Hashing.h"

namespace LibSWBF2::Chunks::LVL::common
{
    void PROP::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void PROP::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void PROP::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_PropertyName = stream.ReadUInt32();
        m_Value = stream.ReadString();
	std::replace(std::begin(m_Value), std::end(m_Value), '\xa0', '\x20');

        BaseChunk::EnsureEnd(stream);
    }

    std::string PROP::ToString() const
    {
	    std::string name;
        if (!FNV::Lookup(m_PropertyName, name))
        {
            name = std::to_string(m_PropertyName);
        }

        return fmt::format(
            "Property Name: {}\n"
            "Value: {}",
            name,
            m_Value
        );
    }
}
