#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/RawData.h"
#include "Chunks/LVL/common/DATA.h"
#include "prim.h"
#include "Types/Enums.h"


namespace LibSWBF2::Chunks::LVL::prim
{
	struct LIBSWBF2_API DATA_PRIM : public common::DATA
	{
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		Types::String ToString() override;

		uint32_t m_PrimitiveType;
		float_t m_Field1, m_Field2, m_Field3;
	};
}