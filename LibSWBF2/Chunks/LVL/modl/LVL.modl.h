#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "INFO.h"
#include "modl.segm.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::modl
{
	struct LIBSWBF2_API modl : public GenericChunk<"modl"_m>
	{
	public:
		STR<"NAME"_m>* p_Name;
		STR<"NODE"_m>* p_Node;	// parent class?
		INFO* p_Info;

		std::vector<segm*> m_Segments;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
