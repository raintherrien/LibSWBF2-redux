#pragma once
#include "Chunks/GenericChunk.h"
#include "DATA.h"

/*
This is a common chunk type, but for now its
usefulness as a base class is unclear...
*/
namespace LibSWBF2::Chunks::LVL::common
{
	struct LIBSWBF2_API SCOP : public GenericChunk<"SCOP"_m>{};
}
