#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::coll
{
    struct LIBSWBF2_API INFO_coll : public GenericChunk
    {
    public:
	    void RefreshSize() override;
	    void WriteToStream(FileWriter& stream) override;
	    void ReadFromStream(FileReader& stream) override;

	    std::string ToString() const override;

	    uint32_t GetHeader() override { return "INFO"_m; }

            Vector3 m_BBoxMin, m_BBoxMax;
            uint32_t m_NumVerts, m_NumInternalNodes, m_NumLeafNodes, m_NumIndiciesInTree;
    };
}
