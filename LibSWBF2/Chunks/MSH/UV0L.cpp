#include "pch.h"
#include "UV0L.h"
#include "FileReader.h"
#include "FileWriter.h"

namespace LibSWBF2::Chunks::MSH
{
	void UV0L::RefreshSize()
	{
		m_Size = (ChunkSize)(sizeof(uint32_t) + m_UVs.size() * Vector2::SIZE);
	}

	void UV0L::WriteToStream(FileWriter& stream)
	{
		BaseChunk::WriteToStream(stream);
		stream.WriteUInt32((uint32_t)m_UVs.size());

		for (size_t i = 0; i < m_UVs.size(); ++i)
		{
			m_UVs[i].WriteToStream(stream);
		}
	}

	void UV0L::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		uint32_t numVertices = stream.ReadUInt32();

		m_UVs.clear();
		m_UVs.reserve(numVertices);

		for (uint32_t i = 0; i < numVertices; ++i)
		{
			Vector2& uv = m_UVs.emplace_back();
			uv.ReadFromStream(stream);
		}

		BaseChunk::EnsureEnd(stream);
	}
}
