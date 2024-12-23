#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct LIBSWBF2_API SIZE : public GenericChunk
	{
	public:
		Vector3 m_Dimensions;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "SIZE"_m; }
	};
}
