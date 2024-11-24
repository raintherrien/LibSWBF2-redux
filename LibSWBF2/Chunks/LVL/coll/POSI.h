#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::coll
{
    struct LIBSWBF2_API POSI : public GenericChunk<"POSI"_m>
    {
    public:
	    void RefreshSize() override;
	    void WriteToStream(FileWriter& stream) override;
	    void ReadFromStream(FileReader& stream) override;

	    std::string ToString() const override;

	    std::vector<Vector3> m_Verts;
    };
}
