#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/RawData.h"
#include "Chunks/LVL/common/DATA.h"
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::prim
{
	struct LIBSWBF2_API DATA_PRIM : public GenericChunk<"DATA"_m>
	{
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		ECollisionPrimitiveType m_PrimitiveType;
		float_t m_Field1, m_Field2, m_Field3;
	};
}
