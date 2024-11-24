#pragma once
#include <vector>

namespace LibSWBF2::Types
{
	struct LIBSWBF2_API Polygon
	{
		Polygon() = default;
		~Polygon() = default;

		std::vector<uint16_t> m_VertexIndices;
	};
}
