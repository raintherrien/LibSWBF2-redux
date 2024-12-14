#pragma once
#include "req.h"
#include "Types/Color4u8.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::terrain
{
	struct tern;
}


namespace LibSWBF2::Wrappers
{
	using Types::Vector2;
	using Types::Vector3;
	using Types::Color4u8;
	
	class Level;

	class LIBSWBF2_API Terrain
	{

	typedef LibSWBF2::Chunks::LVL::terrain::tern tern;

	private:
		friend Level;

	private:
		tern* p_Terrain;

		// the whole terrain, stiched together from all the patches
		std::vector<Vector3> m_Positions;
		std::vector<Vector3> m_Normals;
		std::vector<Vector2> m_TexCoords;
		std::vector<Color4u8> m_Colors;

		// TODO: Please don't lazy initialize these...
		mutable std::vector<float_t> p_HeightMap;
		mutable std::vector<uint8_t> m_BlendMap;

	public:
		static bool FromChunk(tern* terrainChunk, Terrain& out);

		std::string GetName() const;

		// count is number of indices, NOT number of bytes!
		std::vector<uint32_t> GetIndexBuffer(ETopology requestedTopology) const;

		// count is number of vectors, NOT number of bytes!
		std::vector<Vector3> GetVertexBuffer() const;

		// count is number of vectors, NOT number of bytes!
		void GetNormalBuffer(uint32_t& count, const Vector3*& normalBuffer) const;

		// count is number of colors, NOT number of bytes!
		void GetColorBuffer(uint32_t& count, const Color4u8*& colorBuffer) const;

		// count is number of vectors, NOT number of bytes!
		std::vector<Vector2> GetUVBuffer() const;

		void GetHeightMap(uint32_t& dim, uint32_t& dimScale, float_t*& heightData) const;

		std::vector<uint8_t> GetBlendMap(uint32_t& dim, uint32_t& numLayers) const;

		void GetHeightBounds(float_t& floor, float_t& ceiling) const;
 
		// Texture name for each terrain layer. Usually 16 layers
		std::vector<std::string> GetLayerTextures() const;
	};
}
