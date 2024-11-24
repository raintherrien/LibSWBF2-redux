#pragma once
#include "req.h"
#include "Types/Color4u8.h"
#include <string>

namespace LibSWBF2
{
	class Container;

	namespace Chunks::LVL::modl
	{
		struct MTRL;
	}
}

namespace LibSWBF2::Wrappers
{
	using Chunks::LVL::modl::MTRL;
	using Types::Color4u8;

	class Level;
	class Segment;
	class Texture;

	class LIBSWBF2_API Material
	{
	private:
		friend Level;
		friend Segment;

		Material() = default;
		~Material() = default;

	private:
		Level* m_MainContainer = nullptr;
		MTRL* p_Material = nullptr;

	public:
		static bool FromChunk(Level* mainContainer, MTRL* materialChunk, Material& out);

		EMaterialFlags GetFlags() const;
		const Color4u8& GetDiffuseColor() const;
		const Color4u8& GetSpecularColor() const;
		uint32_t GetSpecularExponent() const;
		std::string GetAttachedLight() const;
		bool GetTextureName(uint8_t index, std::string& outName) const;

		// will try to resolve within this Level
		const Texture* GetTexture(uint8_t index) const;

		const uint32_t GetFirstParameter() const;
		const uint32_t GetSecondParameter() const;
	};
}
