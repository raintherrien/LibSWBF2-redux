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

	private:
		std::weak_ptr<Level> m_MainContainer;
		std::shared_ptr<MTRL> p_Material;

	public:
		static std::optional<Material> FromChunk(std::shared_ptr<Level> mainContainer, std::shared_ptr<MTRL> materialChunk);

		EMaterialFlags GetFlags() const;
		const Color4u8& GetDiffuseColor() const;
		const Color4u8& GetSpecularColor() const;
		uint32_t GetSpecularExponent() const;
		std::string GetAttachedLight() const;
		bool GetTextureName(uint8_t index, std::string& outName) const;

		// will try to resolve within this Level
		const Texture* GetTexture(uint8_t index) const;

		uint32_t GetFirstParameter() const;
		uint32_t GetSecondParameter() const;
	};
}
