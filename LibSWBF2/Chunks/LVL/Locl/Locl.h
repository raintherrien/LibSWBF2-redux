#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "Locl.BODY.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::Localization
{
	struct LIBSWBF2_API Locl : public GenericChunk
	{
	public:
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<BODY> p_Body;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "Locl"_m; }
	};
}
