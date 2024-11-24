#pragma once
#include "Chunks/BaseChunk.h"
#include <vector>

namespace LibSWBF2::Chunks::MSH
{
	struct GEOM;

	struct LIBSWBF2_API ENVL : public BaseChunk
	{
		ENVL() = default;
		~ENVL() = default;

		std::vector<uint32_t> m_ModelIndices;

	protected:
		friend GEOM;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};
}
