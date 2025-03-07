#include "pch.h"
#include "GEOM.h"
#include "FileReader.h"
#include "FileWriter.h"

namespace LibSWBF2::Chunks::MSH
{
	void GEOM::RefreshSize()
	{
		m_BoundingBox.RefreshSize();
		m_Envelope.RefreshSize();
		m_Size = m_BoundingBox.GetDataSize() + m_Envelope.GetDataSize();

		for (size_t i = 0; i < m_Segments.size(); ++i)
		{
			m_Segments[i].RefreshSize();
			m_Size += m_BoundingBox.GetDataSize();
		}
	}

	void GEOM::WriteToStream(FileWriter& stream)
	{
		BaseChunk::WriteToStream(stream);
		m_BoundingBox.WriteToStream(stream);

		for (size_t i = 0; i < m_Segments.size(); ++i)
		{
			m_Segments[i].WriteToStream(stream);
		}

		m_Envelope.WriteToStream(stream);
	}

	void GEOM::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);

		while (ThereIsAnother(stream))
		{
			ChunkHeader head = stream.ReadChunkHeader(true);

			if (head == "BBOX"_h)
			{
				m_BoundingBox.ReadFromStream(stream);
			}
			else if (head == "SEGM"_h)
			{
				SEGM& segment = m_Segments.emplace_back();
				segment.ReadFromStream(stream);
			}
			else if (head == "ENVL"_h)
			{
				m_Envelope.ReadFromStream(stream);
			}
			else
			{
				SkipChunk(stream);
			}
		}

		BaseChunk::EnsureEnd(stream);
	}
}
