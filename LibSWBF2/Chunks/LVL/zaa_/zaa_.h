#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "BIN_.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct LIBSWBF2_API zaa_ : public GenericChunk
	{
	public:
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<BIN_> p_Bin;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "zaa_"_m; }
	};
}
