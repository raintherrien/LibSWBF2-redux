#pragma once
#include "Material.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::modl
{
	struct segm;
	struct VBUF;
}

namespace LibSWBF2::Wrappers
{
	using Types::Vector2;
	using Types::Vector3;
	using Chunks::LVL::modl::segm;
	using Chunks::LVL::modl::VBUF;

	class Level;
	class Model;


	#pragma pack(push,1)
	struct LIBSWBF2_API VertexWeight
	{
		float_t m_WeightValue;
		uint8_t m_BoneIndex;
	};
	#pragma pack(pop)

	class LIBSWBF2_API Segment
	{
	private:
		friend Model;

	private:
		segm* p_Segment;
		VBUF* p_VertexBuffer;

		Material m_Material;
		std::vector<VertexWeight> m_VertexWeights;

	public:
		Segment() = default;
		~Segment() = default;

		static bool FromChunk(Level* mainContainer, segm* segmentChunk, Segment& out);

		ETopology GetTopology() const;
		bool ContainsWeights() const;

		const Material& GetMaterial() const;

		// count is number of indices, NOT number of bytes!
		void GetIndexBuffer(uint32_t& count, uint16_t*& indexBuffer) const;

		// count is number of vectors, NOT number of bytes!
		void GetVertexBuffer(uint32_t& count, Vector3*& vertexBuffer) const;

		// count is number of vectors, NOT number of bytes!
		void GetNormalBuffer(uint32_t& count, Vector3*& normalBuffer) const;

		// count is number of vectors, NOT number of bytes!
		void GetUVBuffer(uint32_t& count, Vector2*& uvBuffer) const;

		// count corresponmds to number of vertices, NOT number of bytes!
		// returns false if there are no vertex weights present
		bool GetVertexWeights(uint32_t& count, const VertexWeight*& weightBuffer) const;

		// returns the skeleton bone this segment belongs to
		std::string GetBone() const;

		// Used to indicate special meaning. Two known uses are for 
		// indicating override_texture capability and scrolling
		// wheel textures a la snailtank
		std::string GetTag() const;

		// Some weighted VBUF data is pre-transformed to zeroed bone transforms
		bool IsPretransformed() const;
	};
}
