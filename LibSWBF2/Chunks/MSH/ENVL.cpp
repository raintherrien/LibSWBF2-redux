#include "pch.h"
#include "ENVL.h"
#include "FileReader.h"
#include "FileWriter.h"

namespace LibSWBF2::Chunks::MSH
{
	void ENVL::RefreshSize()
	{
		m_Size = sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t);
	}

	void ENVL::WriteToStream(FileWriter& stream)
	{
		BaseChunk::WriteToStream(stream);
		stream.WriteUInt32((uint32_t)m_ModelIndices.size());

		for (size_t i = 0; i < m_ModelIndices.size(); ++i)
		{
			stream.WriteUInt32((uint32_t)m_ModelIndices[i]);
		}
	}

	void ENVL::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		uint32_t length = stream.ReadUInt32();

		m_ModelIndices.clear();
		m_ModelIndices.reserve(length);

		for (size_t i = 0; i < length; ++i)
		{
			m_ModelIndices.push_back(stream.ReadUInt32());
		}

		BaseChunk::EnsureEnd(stream);
	}
}
