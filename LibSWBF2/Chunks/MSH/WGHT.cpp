#include "pch.h"
#include "WGHT.h"
#include "FileReader.h"
#include "FileWriter.h"

namespace LibSWBF2::Chunks::MSH
{
	void WGHT::RefreshSize()
	{
		m_Size = (ChunkSize)(m_Weights.size() * BoneWeight::SIZE);
	}

	void WGHT::WriteToStream(FileWriter& stream)
	{
		BaseChunk::WriteToStream(stream);
		stream.WriteUInt32((uint32_t)m_Weights.size());

		for (size_t i = 0; i < m_Weights.size(); ++i)
		{
			m_Weights[i].WriteToStream(stream);
		}
	}

	void WGHT::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		uint32_t WeightSize = stream.ReadUInt32();	// stores the number of indices (4 weight multiples), not the total amount of weights!

		m_Weights.clear();
		m_Weights.reserve(WeightSize);

		for (uint32_t i = 0; i < WeightSize; ++i)
		{
			VertexWeights& weights = m_Weights.emplace_back();
			weights.ReadFromStream(stream);
		}

		BaseChunk::EnsureEnd(stream);
	}
}
