#include "pch.h"
#include "TREE.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"


namespace LibSWBF2::Chunks::LVL::coll
{
	void TREE::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void TREE::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void TREE::ReadFromStream(FileReader& stream)
	{	   
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		ChunkHeader nextHeader;

		int index = 0;

		while(ThereIsAnother(stream))
		{
			nextHeader = stream.ReadChunkHeader(true);

			if (nextHeader == "NODE"_h)
			{
				std::shared_ptr<TREE_NODE> tempNode = ReadChild<TREE_NODE>(stream);
				tempNode -> m_FlattenedTreeIndex = index++;
				m_Nodes.push_back(tempNode);
			}
			else if (nextHeader == "LEAF"_h)
			{
				std::shared_ptr<TREE_LEAF> tempLeaf = ReadChild<TREE_LEAF>(stream);
				tempLeaf -> m_FlattenedTreeIndex = index++;
				m_Leaves.push_back(tempLeaf);
			}
			else
			{
				LIBSWBF2_LOG_WARN("Irregular TREE child: {}", nextHeader.ToString());
				ReadChild<GenericChunk>(stream);
			}
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string TREE::ToString() const 
	{
		return fmt::format("{} internal nodes, {} leaf nodes",
							m_Nodes.size(),
							m_Leaves.size());
	}
}
