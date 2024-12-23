#pragma once
#include "Types/Enums.h"
#include <memory>
#include <optional>
#include <string>

namespace LibSWBF2::Chunks::LVL::texture
{
	struct tex_;
	struct FMT_;
}

namespace LibSWBF2::Wrappers
{
	using Chunks::LVL::texture::tex_;
	using Chunks::LVL::texture::FMT_;

	class Level;

	class LIBSWBF2_API Texture
	{
	private:
		friend Level;

		std::shared_ptr<tex_> p_Texture;
		std::shared_ptr<FMT_> p_FMT;

	public:
		static std::optional<Texture> FromChunk(std::shared_ptr<tex_> textureChunk);

		std::string GetName() const;
		uint32_t GetNumMipMaps() const;

		bool GetImageData(ETextureFormat format, uint8_t mipLevel, uint16_t& width, uint16_t& height, const uint8_t*& data) const;
	};
}
