#pragma once
#include "Chunks/GenericChunk.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct anmh_INFO;

	struct LIBSWBF2_API anmh : public GenericChunk
	{
		std::shared_ptr<anmh_INFO> p_Info;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		uint32_t GetHeader() override { return "anmh"_m; }
	};
}
