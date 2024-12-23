#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "XFRM.h"
#include "SIZE.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct LIBSWBF2_API INFO : public GenericChunk
	{
		std::shared_ptr<STR<"TYPE"_m>> p_Type;
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<XFRM> p_XFRM;				// contains rotation and position
		std::shared_ptr<SIZE> p_SIZE;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};
}
