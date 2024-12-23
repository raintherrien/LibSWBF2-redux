#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "INFO.h"
#include "modl.segm.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::modl
{
	struct LIBSWBF2_API modl : public GenericChunk
	{
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<STR<"NODE"_m>> p_Node;	// parent class?
		std::shared_ptr<INFO> p_Info;

		std::vector<std::shared_ptr<segm>> m_Segments;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "modl"_m; }
	};
}
