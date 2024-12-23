#include "pch.h"
#include "wrld.h"
#include "InternalHelpers.h"
#include "Logging/Logger.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
	void wrld::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void wrld::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void wrld::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		p_Name = ReadChild<STR<"NAME"_m>>(stream);

		// since in wrld, some chunks are optional, we have to crawl dynamically
		while (ThereIsAnother(stream))
		{
			ChunkHeader nextHead = stream.ReadChunkHeader(true);
			if (nextHead == "TNAM"_h)
			{
				p_TerrainName = ReadChild<STR<"TNAM"_m>>(stream);
			}
			else if (nextHead == "SNAM"_h)
			{
				p_SkyName = ReadChild<STR<"SNAM"_m>>(stream);
			}
			else if (nextHead == "inst"_h)
			{
				m_Instances.emplace_back(ReadChild<inst>(stream));
			}
			else if (nextHead == "regn"_h)
			{
				m_Regions.emplace_back(ReadChild<regn>(stream));
			}
			else if (nextHead == "anim"_h)
			{
				m_Animations.emplace_back(ReadChild<anim>(stream));
			}
			else if (nextHead == "anmg"_h)
			{
				m_AnimationGroups.emplace_back(ReadChild<anmg>(stream));
			}
			else if (nextHead == "anmh"_h)
			{
				m_AnimationHierarchies.emplace_back(ReadChild<anmh>(stream));
			}
			else if (nextHead == "BARR"_h)
			{
				m_Barriers.emplace_back(ReadChild<BARR>(stream));
			}
			else if (nextHead == "Hint"_h)
			{
				m_HintNodes.emplace_back(ReadChild<Hint>(stream));
			}
			else
			{
				(void) ReadChild<GenericChunk>(stream);
			}
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string wrld::ToString() const
	{
		return fmt::format(
			"World Name: {}\n\n"
			"Terrain Name: {}\n"
			"Sky Name: {}\n"
			"Number of object instances: {}\n",
			p_Name->m_Text,
			p_TerrainName ? p_TerrainName->m_Text : "",
			p_SkyName ? p_SkyName->m_Text : "",
			m_Instances.size()
		);
	}
}
