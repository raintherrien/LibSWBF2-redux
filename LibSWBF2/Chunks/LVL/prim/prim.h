#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "prim.INFO.h"
#include "MASK.h"
#include "prim.DATA.h"
#include "../wrld/XFRM.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::prim
{
	struct LIBSWBF2_API prim : public GenericChunk
	{
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		std::shared_ptr<INFO_prim> p_InfoChunk;

		std::vector<std::shared_ptr<STR<"NAME"_m>>> m_PrimitiveNAMEs;
		std::vector<std::shared_ptr<MASK>> m_PrimitiveMASKs;
		std::vector<std::shared_ptr<STR<"PRNT"_m>>> m_PrimitivePRNTs;
		std::vector<std::shared_ptr<wrld::XFRM>> m_PrimitiveXFRMs;
		std::vector<std::shared_ptr<DATA_PRIM>> m_PrimitiveDATAs;

		uint32_t GetHeader() override { return "prim"_m; }
	};
}
