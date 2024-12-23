#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "segm.INFO.h"
#include <vector>

namespace LibSWBF2::Chunks::LVL::modl
{
	struct MTRL;
	struct SKIN;
	struct BMAP;
	struct IBUF;
	struct TNAM;
	struct VBUF;

	struct LIBSWBF2_API segm : public GenericChunk
	{
		std::shared_ptr<segment::INFO> p_Info;
		std::shared_ptr<MTRL> p_Material;
		std::shared_ptr<STR<"RTYP"_m>> p_RenderType;	// string seems to represent an enum
		std::shared_ptr<IBUF> p_IndexBuffer;
		std::shared_ptr<STR<"BNAM"_m>> p_Parent;
		std::shared_ptr<STR<"MNAM"_m>> p_Tag; 			// Still don't know MNAM this means, ModelName???
		std::shared_ptr<SKIN> p_Skin;					// OPTIONAL
		std::shared_ptr<BMAP> p_BoneMap;				// OPTIONAL. This mapper seems unnecessary to me...

		std::vector<std::shared_ptr<VBUF>> m_VertexBuffers;
		std::vector<std::shared_ptr<TNAM>> m_Textures;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		uint32_t GetHeader() override { return "segm"_m; }
	};
}
