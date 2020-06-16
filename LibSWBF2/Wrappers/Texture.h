#pragma once
#include "Chunks/LVL/tex_/tex_.h"
#include "Types/Enums.h"

namespace LibSWBF2::Wrappers
{
	using Chunks::LVL::texture::tex_;
	using Chunks::LVL::texture::FMT_;
	using LibSWBF2::Types::String;

	class LIBSWBF2_API Texture
	{
	public:
		Texture() = default;
		~Texture() = default;

	private:
		tex_* p_Texture;
		FMT_* p_FMT;

	public:
		static bool FromChunk(tex_* textureCHunk, Texture& out);

		String GetName() const;
		uint32_t GetNumMipMaps() const;

		bool GetImageData(ETextureFormat format, uint8_t mipLevel, uint16_t& width, uint16_t& height, uint8_t*& data) const;
	};
}