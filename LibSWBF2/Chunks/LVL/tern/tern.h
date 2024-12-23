#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "tern.INFO.h"
#include "PCHS.h"
#include "LTEX.h"
#include "DTEX.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::terrain
{
	struct LIBSWBF2_API tern : public GenericChunk
	{
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<STR<"DTLX"_m>> p_DetailTexture;
		std::shared_ptr<INFO> p_Info;
		std::shared_ptr<LTEX> p_LayerTextures;
		std::shared_ptr<DTEX> p_LayerDetailTextures;
		std::shared_ptr<PCHS> p_Patches;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "tern"_m; }
	};
}
