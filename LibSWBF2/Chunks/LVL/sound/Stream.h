#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/RawData.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::sound
{
	struct StreamInfo;
	struct StreamData;

	struct LIBSWBF2_API Stream : public GenericChunk
	{
		std::shared_ptr<StreamInfo> p_Info;
		std::shared_ptr<StreamData> p_Data;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		static FNVHash PeekStreamName(FileReader& stream);

		std::string ToString() const override;
		uint32_t GetHeader() override { return "Stream"_fnv; }
	};
}
