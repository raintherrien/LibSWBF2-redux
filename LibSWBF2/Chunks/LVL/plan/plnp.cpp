#include "pch.h"
#include "plnp.h"
#include "FileReader.h"
#include "InternalHelpers.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"

void LibSWBF2::Chunks::plnp::plnp::ReadFromStream(FileReader& stream)
{
    BaseChunk::ReadFromStream(stream);
    Check(stream);

    ChunkHeader nextHeader;

    uint8_t NumDynamicGroups = stream.ReadByte();

    for (int i = 0; i < NumDynamicGroups; i++)
    {
        m_NumConnectionsInEachGroup.push_back(stream.ReadByte());
    }

    uint8_t NumHashes = stream.ReadByte();

    for (int i = 0; i < NumHashes; i++)
    {
        m_ConnectionHashes.push_back(stream.ReadByte());
    }

    /*
    Dk how exactly to interpret the rest, but some easy clues will be
    investigated at a later time if this data is needed
    */

    BaseChunk::EnsureEnd(stream);
}


std::string LibSWBF2::Chunks::plnp::plnp::ToString() const
{
	std::string rep = fmt::format("Previous planning dataset has {0} dynamic groups:\n", m_NumConnectionsInEachGroup.size());
    for (int i = 0; i < m_NumConnectionsInEachGroup.size(); i++)
    {
        rep = rep + fmt::format("  Group {0} has {1} connections,\n", i, m_NumConnectionsInEachGroup[i]);
    }
    return rep;
}

