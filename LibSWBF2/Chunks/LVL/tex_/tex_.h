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
	struct LIBSWBF2_API tex_ : public GenericChunk<"tex_"_m>
	{
	public:
		STR<"NAME"_m>* p_Name;
		texture::INFO* p_Info;
		std::vector<FMT_*> m_FMTs;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
