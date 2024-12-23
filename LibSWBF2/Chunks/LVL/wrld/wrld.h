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
	struct LIBSWBF2_API wrld : public GenericChunk
	{
	public:
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<STR<"TNAM"_m>> p_TerrainName;	// optional chunk, can be nullptr
		std::shared_ptr<STR<"SNAM"_m>> p_SkyName;		// optional chunk, can be nullptr

		// a.k.a. objects placed into the world
		std::vector<std::shared_ptr<inst>> m_Instances;

		std::vector<std::shared_ptr<regn>> m_Regions;

		std::vector<std::shared_ptr<anim>> m_Animations;
		std::vector<std::shared_ptr<anmg>> m_AnimationGroups;
		std::vector<std::shared_ptr<anmh>> m_AnimationHierarchies;

		std::vector<std::shared_ptr<BARR>> m_Barriers;
		std::vector<std::shared_ptr<Hint>> m_HintNodes;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "wrld"_m; }
	};
}
