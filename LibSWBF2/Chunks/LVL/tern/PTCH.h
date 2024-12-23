#pragma once
#include "Chunks/GenericChunk.h"
#include "PTCH.VBUF.h"
#include "PTCH.IBUF.h"
#include "PTCH.INFO.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::terrain
{
	struct LIBSWBF2_API PTCH : public GenericChunk
	{
		std::shared_ptr<VBUF> m_TextureBuffer;
		std::shared_ptr<VBUF> m_GeometryBuffer;
		std::shared_ptr<VBUF> m_TextureExtraBuffer;

		std::shared_ptr<IBUF> m_GeometryIndexBuffer;
		std::shared_ptr<IBUF> m_TextureExtraIndexBuffer;

		std::shared_ptr<PTCH_INFO> p_PatchInfo;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "PTCH"_m; }
	};
}
