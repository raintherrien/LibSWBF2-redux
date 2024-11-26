#include "pch.h"
#include "TREE.LEAF.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"

#include <string>


namespace LibSWBF2::Chunks::LVL::coll
{
	void TREE_LEAF::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void TREE_LEAF::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void TREE_LEAF::ReadFromStream(FileReader& stream)
	{       
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        uint32_t numIndicies = (uint32_t) stream.ReadByte();

        stream.SkipBytes(6); //unknown

        for (uint32_t i = 0; i < numIndicies; i++)
        {
        	m_Indicies.push_back(stream.ReadUInt16());
        }

		BaseChunk::EnsureEnd(stream);
	}

	
	std::string TREE_LEAF::ToString() const 
    {
        std::string stringRep = "{";

        for (int i = 0; i < m_Indicies.size(); i++)
        {
        	stringRep += fmt::format(" {}", m_Indicies[i]);
        }

        stringRep += " }";
        return stringRep;
    }
}
