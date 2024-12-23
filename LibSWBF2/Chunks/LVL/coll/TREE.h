#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "TREE.NODE.h"
#include "TREE.LEAF.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::coll
{
	struct LIBSWBF2_API TREE : public GenericChunk
	{
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "TREE"_m; }

		std::vector<std::shared_ptr<TREE_NODE>> m_Nodes;
		std::vector<std::shared_ptr<TREE_LEAF>> m_Leaves;
	};
}
