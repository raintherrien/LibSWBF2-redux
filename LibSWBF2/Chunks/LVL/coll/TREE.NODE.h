#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::coll
{
	struct LIBSWBF2_API TREE_NODE : public GenericChunk
	{

	friend struct TREE;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "NODE"_m; }

		Vector3u8 m_VecLower, m_VecUpper;

	private:
		uint32_t m_FlattenedTreeIndex;
	};
}
