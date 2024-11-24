#pragma once
#include "Chunks/BaseChunk.h"
#include "MATD.h"
#include <vector>

namespace LibSWBF2::Chunks::MSH
{
	struct MSH2;

	struct LIBSWBF2_API MATL : public BaseChunk
	{
		MATL() = default;
		~MATL() = default;

		std::vector<MATD> m_Materials;

	protected:
		friend MSH2;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};
}
