#pragma once
#include "Chunks/BaseChunk.h"
#include "Types/Polygon.h"
#include <vector>

namespace LibSWBF2::Chunks::MSH
{
	using LibSWBF2::Types::Polygon;

	struct SEGM;

	struct LIBSWBF2_API STRP : public BaseChunk
	{
		STRP() = default;
		~STRP() = default;

		void CalcPolygons();

		// List of Triangle Strips. Each entry contains a vertex index (16 bit unsigned int)
		// Triangles are listed CW, CCW, CW, CCW, ...
		// The start of each Strip is indicated by two entries in a row where the high bit is set (0x8000)
		std::vector<uint16_t> m_Triangles;

		// Converted Triangle Strips to Polygons. Each polygon consits of 3 vertex indices
		// Call CalcPolygons() beforehand!
		std::vector<Polygon> m_Polygons;

	protected:
		friend SEGM;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};
}
