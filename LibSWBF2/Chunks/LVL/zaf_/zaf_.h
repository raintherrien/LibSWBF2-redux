#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct ZAF_BIN_;

	struct LIBSWBF2_API zaf_ : public GenericChunk
	{
	public:
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<ZAF_BIN_> p_Bin;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "zaf_"_m; }
	};
}
