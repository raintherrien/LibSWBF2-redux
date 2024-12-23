#pragma once
#include "Chunks/GenericChunk.h"
#include "DirectX/D3D9FORMAT.h"
#include "Chunks/STR.h"
#include "tex_.INFO.h"
#include "FMT_.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::texture
{
	struct LIBSWBF2_API tex_ : public GenericChunk
	{
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<texture::INFO> p_Info;
		std::vector<std::shared_ptr<FMT_>> m_FMTs;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "tex_"_m; }
	};
}
