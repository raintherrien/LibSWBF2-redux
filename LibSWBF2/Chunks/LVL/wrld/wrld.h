#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "Chunks/STRMULT.h"
#include "inst.h"
#include "regn.h"
#include "anmg.h"
#include "anim.h"
#include "anmh.h"
#include "BARR.h"
#include "Hint.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct LIBSWBF2_API wrld : public GenericChunk<"wrld"_m>
	{
	public:
		STR<"NAME"_m>* p_Name;
		STR<"TNAM"_m>* p_TerrainName;	// optional chunk, can be nullptr
		STR<"SNAM"_m>* p_SkyName;		// optional chunk, can be nullptr

		// a.k.a. objects placed into the world
		std::vector<inst*> m_Instances;

		std::vector<regn*> m_Regions;

		std::vector<anim*> m_Animations;
		std::vector<anmg*> m_AnimationGroups;
		std::vector<anmh*> m_AnimationHierarchies;

		std::vector<BARR*> m_Barriers;
		std::vector<Hint*> m_HintNodes;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
