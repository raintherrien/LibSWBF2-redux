#include "pch.h"
#include "TREE.NODE.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"


namespace LibSWBF2::Chunks::LVL::coll
{
	void TREE_NODE::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void TREE_NODE::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void TREE_NODE::ReadFromStream(FileReader& stream)
	{       
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_VecLower.ReadFromStream(stream);
        m_VecUpper.ReadFromStream(stream);

		BaseChunk::EnsureEnd(stream);
	}

	std::string TREE_NODE::ToString() const 
    {
        return fmt::format("Lower vector: {}\nHigher vector: {}",
        				    m_VecLower.ToString(),
        				    m_VecUpper.ToString());
    }
}
