#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/RawData.h"
#include "Types/Enums.h"

namespace LibSWBF2::Chunks::LVL::common
{
	struct LIBSWBF2_API DATA : public GenericChunk
	{
		static uint32_t PeekDATATag(FileReader& stream);
		uint32_t GetHeader() override { return "DATA"_m; }
	};
}
