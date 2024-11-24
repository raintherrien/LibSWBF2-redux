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
    struct LIBSWBF2_API prim : public GenericChunk<"prim"_m>
    {
	    void RefreshSize() override;
	    void WriteToStream(FileWriter& stream) override;
	    void ReadFromStream(FileReader& stream) override;

	    std::string ToString() const override;
        
        INFO_prim *p_InfoChunk;  
        
	std::vector<STR<"NAME"_m> *> m_PrimitiveNAMEs;
	std::vector<MASK *>          m_PrimitiveMASKs;
	std::vector<STR<"PRNT"_m> *> m_PrimitivePRNTs;  
	std::vector<wrld::XFRM *>    m_PrimitiveXFRMs;
	std::vector<DATA_PRIM *>     m_PrimitiveDATAs;
    };
}
