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

	struct LIBSWBF2_API segm : public GenericChunk<"segm"_m>
	{
	public:
		segment::INFO* p_Info;
		MTRL* p_Material;
		STR<"RTYP"_m>* p_RenderType;	// string seems to represent an enum
		IBUF* p_IndexBuffer;
		STR<"BNAM"_m>* p_Parent;
		STR<"MNAM"_m>* p_Tag; 			// Still don't know MNAM this means, ModelName???
		SKIN* p_Skin;					// OPTIONAL
		BMAP* p_BoneMap;				// OPTIONAL. This mapper seems unnecessary to me...

		std::vector<VBUF*> m_VertexBuffers;
		std::vector<TNAM*> m_Textures;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};
}
