#include "pch.h"
#include "anmg.INFO.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
    void anmg_INFO::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anmg_INFO::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anmg_INFO::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_Name = stream.ReadString();

        m_PlayAtStart = stream.ReadByte();
        m_StopOnControl = stream.ReadByte();

        BaseChunk::EnsureEnd(stream);
    }

    std::string anmg_INFO::ToString() const
    {
	    std::string rep = fmt::format("Name: {}, Is Played At Start? {}, Disabled When Controlled? {}", 
                        m_Name, 
                        m_PlayAtStart == 1,
                        m_StopOnControl == 1);
        return rep;
    }
}
