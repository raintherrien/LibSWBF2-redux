#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "Chunks/STRMULT.h"
#include "skel.INFO.h"
#include "skel.XFRM.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::skel
{
	struct LIBSWBF2_API skel : public GenericChunk
	{
	public:
		std::shared_ptr<INFO> p_Info;
		std::shared_ptr<STRMULT<"NAME"_m>> p_BoneNames;
		std::shared_ptr<STRMULT<"PRNT"_m>> p_BoneParents;
		std::shared_ptr<XFRM> p_BoneTransforms;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "skel"_m; }
	};
}
