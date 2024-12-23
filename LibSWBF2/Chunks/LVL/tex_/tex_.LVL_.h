#pragma once
#include "Chunks/GenericChunk.h"
#include "LVL_.INFO.h"
#include "BODY.h"

namespace LibSWBF2::Chunks::LVL::LVL_texture
{
	struct LIBSWBF2_API LVL_ : public GenericChunk
	{
		std::shared_ptr<INFO> p_Info;
		std::shared_ptr<BODY> p_Body;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		uint32_t GetHeader() override { return "LVL_"_m; }
	};
}
