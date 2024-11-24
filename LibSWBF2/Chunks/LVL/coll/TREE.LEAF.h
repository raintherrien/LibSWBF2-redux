#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::coll
{
    struct LIBSWBF2_API TREE_LEAF : public GenericChunk<"LEAF"_m>
    {

    friend struct TREE;
    friend struct coll;

    public:
	    void RefreshSize() override;
	    void WriteToStream(FileWriter& stream) override;
	    void ReadFromStream(FileReader& stream) override;

	    std::string ToString() const override;

        uint32_t m_FlattenedTreeIndex;
	std::vector<uint16_t> m_Indicies;
    };
}
