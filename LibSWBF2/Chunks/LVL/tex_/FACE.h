#pragma once
#include "Chunks/GenericChunk.h"
#include "tex_.LVL_.h"
#include <vector>

namespace LibSWBF2::Chunks::LVL::texture
{
	using LVL_texture::LVL_;

	struct LIBSWBF2_API FACE : public GenericChunk
	{
	public:
		std::vector<std::shared_ptr<LVL_>> m_LVLs;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		uint32_t GetHeader() override { return "FACE"_m; }
	};
}
