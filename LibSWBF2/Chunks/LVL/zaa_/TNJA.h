#pragma once
#include "Chunks/GenericChunk.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct LIBSWBF2_API TNJA : public GenericChunk
	{
	public:

		std::vector<CRCChecksum> m_BoneCRCs;
		std::vector<uint32_t>    m_RotationOffsets;
		std::vector<uint32_t>    m_TranslationOffsets;
		std::vector<float_t>     m_TranslationParams;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "TNJA"_m; }
	};
}
