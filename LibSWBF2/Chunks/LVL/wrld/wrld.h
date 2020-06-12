#pragma once
#include "Chunks\GenericChunk.h"
#include "Chunks\STR.h"
#include "inst.h"

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct LIBSWBF2_EXP wrld : public GenericChunk<"wrld"_m>
	{
	public:
		STR<"NAME"_m>* p_Name;
		STR<"TNAM"_m>* p_TerrainName;
		STR<"SNAM"_m>* p_SkyName;

		// a.k.a. objects placed into the world
		List<inst*> m_Instances;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};
}