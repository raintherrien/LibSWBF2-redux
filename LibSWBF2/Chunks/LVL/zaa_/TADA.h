#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::animation
{

	struct LIBSWBF2_API TADA : public GenericChunk
	{

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		~TADA();

		uint8_t *p_DataBuffer = nullptr;
		size_t m_DataBufferLength = 0;

		uint32_t GetHeader() override { return "TADA"_m; }
	};
}
