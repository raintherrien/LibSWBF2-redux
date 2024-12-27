#include "pch.h"
#include "anmh.INFO.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
    void anmh_INFO::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anmh_INFO::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anmh_INFO::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_NumStrings = stream.ReadByte();

        for (int i = 0; i < (int) m_NumStrings; i++)
        {
		std::string newStr = stream.ReadString();
            if (i == 0)
            {
                m_RootName = newStr;
            }
            else 
            {
                m_ChildNames.push_back(newStr);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string anmh_INFO::ToString() const
    {
        if (m_NumStrings == 0)
        {
            return "Empty Hierarchy";
        }
        else 
        {
		std::string rep = fmt::format("Root: {} ", m_RootName);
            for (int i = 0; i < m_ChildNames.size(); i++)
            {
                rep = rep + fmt::format("\n  Child {0}: {1}", i, m_ChildNames[i]);
            }  

            return rep;       
        }
    }
}
