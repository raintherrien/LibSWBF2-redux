#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/RawData.h"
#include "DATA.h"
#include "Types/Enums.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::config
{
	struct LIBSWBF2_API DATA_FLOAT : public DATA_CONFIG
	{
		std::vector<float_t> m_Floats;
	
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	
		std::string ToString() const override;
	};
}
