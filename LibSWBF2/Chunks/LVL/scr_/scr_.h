#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "scr_.INFO.h"
#include "Chunks/RawData.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::script
{
	struct LIBSWBF2_API scr_ : public GenericChunk
	{
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<INFO> p_Info;

		// TODO: are there cases with multiple bodies?
		std::shared_ptr<RawData<"BODY"_m>> p_Body;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "scr_"_m; }
	};
}
