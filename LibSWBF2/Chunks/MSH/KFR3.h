#pragma once
#include "Chunks/BaseChunk.h"
#include "Types/BoneFrames.h"
#include <vector>

namespace LibSWBF2::Chunks::MSH
{
	using LibSWBF2::Types::BoneFrames;

	struct ANM2;

	struct LIBSWBF2_API KFR3 : public BaseChunk
	{
		KFR3() = default;
		~KFR3() = default;

		std::vector<BoneFrames> m_BoneFrames;

	protected:
		friend ANM2;
		
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};
}
