#include "stdafx.h"
#include "MATL.h"

namespace LibSWBF2::Chunks::Mesh
{
	MATL::MATL()
	{

	}

	MATL::~MATL()
	{
		
	}

	void MATL::RefreshSize()
	{
		ChunkSize matChunksSize = 0;
		for (int i = 0; i < m_Materials.size(); ++i)
		{
			m_Materials[i].RefreshSize();
			matChunksSize += m_Materials[i].GetSize();
		}

		m_Size = sizeof(uint32_t) + matChunksSize;
	}

	void MATL::WriteToStream(FileWriter& stream)
	{
		BaseChunk::WriteToStream(stream);
		stream.WriteUInt32((uint32_t)m_Materials.size());

		for (size_t i = 0; i < m_Materials.size(); ++i)
		{
			m_Materials[i].WriteToStream(stream);
		}
	}

	void MATL::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		uint32_t MaterialsSize = stream.ReadUInt32();

		m_Materials.clear();
		m_Materials.reserve(MaterialsSize);

		for (uint32_t i = 0; i < MaterialsSize; ++i)
		{
			m_Materials.emplace_back(MATD());
			m_Materials[m_Materials.size() - 1].ReadFromStream(stream);
		}
	}
}