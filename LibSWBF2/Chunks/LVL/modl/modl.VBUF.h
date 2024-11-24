#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "Types/Enums.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::modl
{
	struct LIBSWBF2_API VBUF : public GenericChunk<"VBUF"_m>
	{
	public:
		uint32_t m_Count;
		uint32_t m_Stride;	// bytes per vertex
		EVBUFFlags m_Flags;
		
		std::vector<Vector3> m_Positions;
		std::vector<Vector3> m_Normals;
		std::vector<Vector3> m_Tangents;
		std::vector<Vector3> m_BiTangents;

		std::vector<Color4u8> m_Colors;
		std::vector<Vector2> m_TexCoords;

		std::vector<Vector3u8> m_BoneIndicies;
		std::vector<Vector3> m_Weights;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
