#include "pch.h"
#include "plan.h"
#include "FileReader.h"
#include "InternalHelpers.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"

#define NAME_LENGTH 16


void LibSWBF2::Chunks::plan::PLAN_INFO::ReadFromStream(FileReader& stream)
{
    BaseChunk::ReadFromStream(stream);
    Check(stream);

    m_iHubCount = stream.ReadInt16();
    m_iConnectionCount = stream.ReadInt16();

    BaseChunk::EnsureEnd(stream);
}


std::string LibSWBF2::Chunks::plan::PLAN_INFO::ToString() const
{
    return fmt::format("Hub count: {0}, Connection count: {1}", 
                    m_iHubCount, m_iConnectionCount);
}



void LibSWBF2::Chunks::plan::plan::ReadFromStream(FileReader& stream)
{
    BaseChunk::ReadFromStream(stream);
    Check(stream);

    ChunkHeader nextHeader;

    while (ThereIsAnother(stream))
    {
        nextHeader = stream.ReadChunkHeader(true);

        if (nextHeader == "INFO"_h)
        {
            m_pInfo = ReadChild<PLAN_INFO>(stream);
        }
        else if (nextHeader == "NODE"_h)
        {
            m_pNode = std::make_shared<NODE>(m_pInfo->m_iHubCount);
            ReadExplicitNoCtor(this, stream, m_pNode);
        }
        else if (nextHeader == "ARCS"_h)
        {
            m_pArcs = std::make_shared<ARCS>(m_pInfo->m_iConnectionCount);
            ReadExplicitNoCtor(this, stream, m_pArcs);
        }
        else
        {
            ReadChild<GenericChunk>(stream);
        }
    }

    BaseChunk::EnsureEnd(stream);
}

void LibSWBF2::Chunks::plan::NODE::ReadFromStream(FileReader& stream)
{
    BaseChunk::ReadFromStream(stream);
    Check(stream);

    for (int i = 0; i < m_iCount; i++)
    {
        m_aHubs.emplace_back().ReadFromStream(stream, m_iCount);
    }

    BaseChunk::EnsureEnd(stream);
}

LibSWBF2::Chunks::plan::NODE::NODE(uint16_t count)
    : m_iCount(count)
{
}


std::string LibSWBF2::Chunks::plan::NODE::ToString() const
{
	std::string rep = fmt::format("{0} hubs:\n", m_iCount);
    for (int i = 0; i < m_aHubs.size(); i++)
    {
        rep = "  " + rep + m_aHubs[i].ToString() + "\n";
    }
    return rep;
}



void LibSWBF2::Chunks::plan::ARCS::ReadFromStream(FileReader& stream)
{
    BaseChunk::ReadFromStream(stream);
    Check(stream);

    for (int i = 0; i < m_iCount; i++)
    {
        m_aConnections.emplace_back().ReadFromStream(stream);
    }

    BaseChunk::EnsureEnd(stream);
}

LibSWBF2::Chunks::plan::ARCS::ARCS(uint16_t count)
    : m_iCount(count)
{
}


std::string LibSWBF2::Chunks::plan::ARCS::ToString() const
{
	std::string rep = fmt::format("{0} connections:\n", m_iCount);
    for (int i = 0; i < m_aConnections.size(); i++)
    {
        rep = "  " + rep + m_aConnections[i].ToString() + "\n";
    }
    return rep;
}
