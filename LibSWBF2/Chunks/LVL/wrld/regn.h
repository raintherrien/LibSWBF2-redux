#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/LVL/common/PROP.h"
#include "wrld.INFO.h"
#include <vector>

namespace LibSWBF2::Chunks::LVL::wrld
{
	using LibSWBF2::Chunks::LVL::wrld::INFO;
	using common::PROP;

	struct LIBSWBF2_API regn : public GenericChunk
	{
		std::shared_ptr<INFO> p_Info;
		std::vector<std::shared_ptr<PROP>> m_Props;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "regn"_m; }
	};
}
