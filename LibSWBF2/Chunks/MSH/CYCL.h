#pragma once
#include "Chunks/BaseChunk.h"
#include "Types/Animation.h"
#include <vector>

namespace LibSWBF2::Chunks::MSH
{
	using Types::Animation;

	struct ANM2;

	struct LIBSWBF2_API CYCL : public BaseChunk
	{
		const int ANIMATION_NAME_STR_SIZE = 64;

		CYCL() = default;
		~CYCL() = default;

		std::vector<Animation> m_Animations;

	protected:
		friend ANM2;
		
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};
}
