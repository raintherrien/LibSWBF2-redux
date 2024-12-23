#include "pch.h"
#include "Texture.h"
#include "InternalHelpers.h"
#include "Chunks/LVL/tex_/tex_.h"
#include "Chunks/LVL/tex_/tex_.LVL_.h"
#include <string>
#include <vector>

namespace LibSWBF2::Wrappers
{
	using Chunks::LVL::LVL_texture::LVL_;

	std::optional<Texture> Texture::FromChunk(std::shared_ptr<tex_> textureChunk)
	{
		Texture out;

		if (textureChunk == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given textureChunk was NULL!");
			return {};
		}

		out.p_Texture = textureChunk;

		std::vector<std::shared_ptr<FMT_>>& fmts = out.p_Texture->m_FMTs;
		if (fmts.size() == 0)
		{
			LIBSWBF2_LOG_WARN("Texture '{}' does not contain any data!", textureChunk->ToString());
			return {};
		}

		// Grab FMT chunk with the least amount of compression
		// TODO: proper format sorting for choosing the right FMT chunk
		out.p_FMT = nullptr;
		for (size_t i = 0; i < fmts.size(); ++i)
		{
			if (fmts[i]->p_Info->m_Format == D3DFMT_DXT3)
			{
				out.p_FMT = fmts[i];
				break;
			}
			if (fmts[i]->p_Info->m_Format == D3DFMT_DXT1)
			{
				out.p_FMT = fmts[i];
				break;
			}
		}

		if (out.p_FMT == nullptr)
		{
			out.p_FMT = fmts[0];
		}

		return out;
	}

	std::string Texture::GetName() const
	{
		return p_Texture->p_Name->m_Text;
	}

	uint32_t Texture::GetNumMipMaps() const
	{
		return p_FMT->p_Info->m_MipmapCount;
	}

	bool Texture::GetImageData(ETextureFormat format, uint8_t mipLevel, uint16_t& width, uint16_t& height, const uint8_t*& data) const
	{
		std::vector<std::shared_ptr<LVL_>>& mipChunks = p_FMT->p_Face->m_LVLs;
		for (size_t i = 0; i < mipChunks.size(); ++i)
		{
			if (mipChunks[i]->p_Info->m_MipLevel == mipLevel)
			{
				return mipChunks[i]->p_Body->GetImageData(format, width, height, data);
			}
		}
		width = 0;
		height = 0;
		data = nullptr;
		return false;
	}
}
