#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "POSI.h"
#include "TREE.h"
#include "../prim/MASK.h"
#include "coll.INFO.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::coll
{
	struct LIBSWBF2_API coll : public GenericChunk
	{
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "coll"_m; }

		std::shared_ptr<STR<"NAME"_m>> p_ChunkName;

		std::shared_ptr<prim::MASK> p_Mask;
		std::shared_ptr<STR<"NODE"_m>> p_NodeName;

		std::shared_ptr<INFO_coll> p_Info;
		std::shared_ptr<TREE> p_Tree;
		std::shared_ptr<POSI> p_Verts;
	};
}
