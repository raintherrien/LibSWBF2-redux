#pragma once
#include "Chunks/GenericChunk.h"
#include "FMT_.INFO.h"
#include "FACE.h"

namespace LibSWBF2::Chunks::LVL::texture
{
	struct LIBSWBF2_API FMT_ : public GenericChunk
	{
		std::shared_ptr<FMT::INFO> p_Info = nullptr;

		// TODO: can there be multiple faces?
		std::shared_ptr<FACE> p_Face = nullptr;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		uint32_t GetHeader() override { return "FMT_"_m; }
	};
}
