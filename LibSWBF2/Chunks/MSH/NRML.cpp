#include "pch.h"
#include "NRML.h"
#include "FileReader.h"
#include "FileWriter.h"

namespace LibSWBF2::Chunks::MSH
{
	void NRML::RefreshSize()
	{
		m_Size = (ChunkSize)(sizeof(uint32_t) + m_Normals.size() * Vector3::SIZE);
	}

	void NRML::WriteToStream(FileWriter& stream)
	{
		BaseChunk::WriteToStream(stream);
		stream.WriteUInt32((uint32_t)m_Normals.size());

		for (size_t i = 0; i < m_Normals.size(); ++i)
		{
			m_Normals[i].WriteToStream(stream);
		}
	}

	void NRML::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		uint32_t numVertices = stream.ReadUInt32();

		m_Normals.clear();
		m_Normals.reserve(numVertices);

		for (uint32_t i = 0; i < numVertices; ++i)
		{
			Vector3& vertex = m_Normals.emplace_back();
			vertex.ReadFromStream(stream);
		}

		BaseChunk::EnsureEnd(stream);
	}
}
