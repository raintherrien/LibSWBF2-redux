#include "pch.h"
#include "STRP.h"
#include "FileReader.h"
#include "FileWriter.h"

namespace LibSWBF2::Chunks::MSH
{
	void STRP::RefreshSize()
	{
		m_Size = (ChunkSize)(sizeof(uint32_t) + m_Triangles.size() * sizeof(uint16_t));
	}

	void STRP::WriteToStream(FileWriter& stream)
	{
		BaseChunk::WriteToStream(stream);
		stream.WriteUInt32((uint32_t)m_Triangles.size());

		for (size_t i = 0; i < m_Triangles.size(); ++i)
		{
			stream.WriteUInt16(m_Triangles[i]);
		}
	}

	void STRP::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		uint32_t numIndices = stream.ReadUInt32();

		m_Triangles.clear();
		m_Triangles.reserve(numIndices);

		for (uint32_t i = 0; i < numIndices; ++i)
		{
			m_Triangles.push_back(stream.ReadUInt16());
		}

		// in some STRP Chunks, there are an additional two bytes (0x00 each)
		// that are NOT included in the overall Chunk size!
		// So lets mark our position and read 16 bits.
		// If it doesn't equal zero, there weren't any additional bytes
		// in the first place and we have to jump back.
		size_t pos = stream.GetPosition();
		if (stream.ReadUInt16() != 0)
		{
			stream.SetPosition(pos);
		}

		BaseChunk::EnsureEnd(stream);
	}

	void STRP::CalcPolygons()
	{
		m_Polygons.clear();

		// in MSH, polygons are defined as triangle strips.
		// so we have to strip them ourselfs
		// triangles are listed CW CCW CW CCW...

		size_t triCount = 0;
		Polygon poly;
		bool CW = true;

		for (size_t i = 0; i < m_Triangles.size(); ++i)
		{
			std::vector<uint16_t>& vInd = poly.m_VertexIndices;
			uint16_t vertex = m_Triangles[i];

			// check if highest bit is set
			// two consecutive indices with the highest bit set indicate the start of a triangle strip
			// so lets AND combine this and the next vertex index and check if the highest bit is still set
			if (i+1 < m_Triangles.size() && (vertex & m_Triangles[i+1]) & 0x8000)
			{
				vInd.clear();
				triCount = 0;
				CW = true;
			}

			// always get the real vertex index if the high bit is set
			// by applying a respective bitmask
			if ((vertex & 0x8000) != 0)
			{
				vertex &= 0x7FFF;
			}

			vInd.push_back(vertex);
			++triCount;

			if (triCount >= 3)
			{
				i -= 2;
				triCount = 0;

				if (!CW)
				{
					// switch vertices if CCW
					uint16_t temp = vInd[vInd.size() - 1];
					vInd[vInd.size() - 1] = vInd[vInd.size() - 3];
					vInd[vInd.size() - 3] = temp;
				}
				CW = !CW;

				m_Polygons.push_back(poly);
				vInd.clear();
			}
		}
	}
}
