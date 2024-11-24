#pragma once
#include "Chunks/BaseChunk.h"
#include <vector>

namespace LibSWBF2::Chunks::MSH
{
	struct SEGM;

	struct LIBSWBF2_API POSL : public BaseChunk
	{
		POSL() = default;
		~POSL() = default;

		std::vector<Vector3> m_Vertices;

	protected:
		friend SEGM;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};
}
