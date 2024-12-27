#include "pch.h"
#include "Material.h"
#include "Texture.h"
#include "Level.h"

#include "Container.h"
#include "InternalHelpers.h"


#include "Chunks/LVL/modl/MTRL.h"
#include "Chunks/LVL/modl/TNAM.h"
#include "Chunks/LVL/modl/modl.segm.h"



namespace LibSWBF2::Wrappers
{
	using LibSWBF2::Chunks::LVL::modl::segm;

	std::optional<Material> Material::FromChunk(std::shared_ptr<Level> mainContainer, std::shared_ptr<MTRL> materialChunk)
	{
		Material out;

		if (mainContainer == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given mainContainer was NULL!");
			return {};
		}
		if (materialChunk == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given materialChunk was NULL!");
			return {};
		}

		out.m_MainContainer = mainContainer;
		out.p_Material = materialChunk;

		return out;
	}

	EMaterialFlags Material::GetFlags() const
	{
		return p_Material->m_Flags;
	}

	const Color4u8& Material::GetDiffuseColor() const
	{
		return p_Material->m_DiffuseColor;
	}

	const Color4u8& Material::GetSpecularColor() const
	{
		return p_Material->m_SpecularColor;
	}

	uint32_t Material::GetSpecularExponent() const
	{
		return p_Material->m_SpecularExponent;
	}

	uint32_t Material::GetFirstParameter() const
	{	
		return p_Material->m_Parameters[0];
	}

	uint32_t Material::GetSecondParameter() const
	{
		return p_Material->m_Parameters[1];
	}

	std::string Material::GetAttachedLight() const
	{
		return p_Material->m_AttachedLight;
	}

	bool Material::GetTextureName(uint8_t index, std::string& outName) const
	{
		segm* segment = dynamic_cast<segm*>(p_Material->GetParent());
		if (segment == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Parent of MTRL is not segm!");
			return false;
		}

		if (index >= segment->m_Textures.size())
		{
			LIBSWBF2_LOG_WARN("Texture index '{}' is out of bounds ({})!", index, segment->m_Textures.size());
			return false;
		}
		outName = segment->m_Textures[index]->m_Name;
		return true;
	}

	const Texture* Material::GetTexture(uint8_t index) const
	{
		std::string textureName;
		if (auto level = m_MainContainer.lock()) {
			if (GetTextureName(index, textureName)) {
				return level->GetTexture(textureName);
			}
		}
		return nullptr;
	}
}
