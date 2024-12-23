#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct TNOJ;

	struct LIBSWBF2_API LEKS : public GenericChunk
	{
	public:

		std::vector<std::shared_ptr<TNOJ>> m_Joints; 

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "LEKS"_m; } //SKEL
	};
}
