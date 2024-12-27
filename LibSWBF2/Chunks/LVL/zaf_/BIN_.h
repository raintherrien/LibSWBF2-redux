#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct LEKS;


	struct LIBSWBF2_API ZAF_BIN_ : public GenericChunk
	{
	public:

		/*
		Should always be 8, as in zaa_.
		If there are other versions relevant to BF1/2, I'm not
		sure how they could be reversed since Zenasset
		doesn't take versioning parameters...
		*/ 
		uint32_t m_Version = 8; 

		// Just a list of joints
		std::shared_ptr<LEKS> p_Skeleton;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "BIN_"_m; }
	};
}
