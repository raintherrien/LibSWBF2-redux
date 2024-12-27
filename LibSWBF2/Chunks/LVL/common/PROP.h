#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::common
{
	struct LIBSWBF2_API PROP : public GenericChunk
	{
	public:
		FNVHash m_PropertyName;
		std::string m_Value;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "PROP"_m; }
	};
}
