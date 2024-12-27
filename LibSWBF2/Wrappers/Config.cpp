#include "req.h"
#include "pch.h"
#include "Config.h"
#include "Chunks/LVL/config/ConfigChunk.h"
#include "Chunks/LVL/config/SCOP.h"
#include "InternalHelpers.h"
#include <string>
#include <vector>

namespace LibSWBF2::Wrappers
{
	using namespace LibSWBF2::Chunks::LVL::config;
	using namespace LibSWBF2::Types;


	/*

	Field

	*/

	std::vector<Field> Field::FieldsFromChunkChildren(GenericBaseChunk *chunk)
	{
		std::vector<Field> fields;
		const std::vector<GenericBaseChunk *>& children = chunk -> GetChildren();
		
		for (uint16_t i = 0; i < children.size(); i++)
		{
			DATA_CONFIG *child = dynamic_cast<DATA_CONFIG *>(children[i]);
			
			if (child == nullptr) continue;
			
			SCOP *scope;
			if (i == children.size() - 1)
			{
				scope = nullptr;
			}
			else
			{
				scope = dynamic_cast<SCOP *>(children[i+1]);
			}
				
			fields.push_back(Field(child, scope));
		}

		return fields;
	}


	Field::Field(DATA_CONFIG *data, SCOP* scop) : m_Scope(scop)
	{
		p_Data = data;
	}

	FNVHash Field::GetNameHash() const
	{
		return p_Data->m_NameHash;
	}

	uint8_t Field::GetNumValues() const
	{
		return p_Data->m_NumValues;
	}

	float_t Field::GetFloat(uint8_t index) const
	{	
		float_t out;
		if (p_Data->GetFloat(out, index))
		{
			return out;
		}
		
		LIBSWBF2_LOG_WARN("Could not get field float value at index {0} in DATA chunk '{1}'!", index, GetName());
		return 0.0f;
	}
	

	uint32_t Field::GetUInt32(uint8_t index) const
	{	
		uint32_t out;
		if (p_Data->GetUInt32(out, index))
		{
			return out;
		}
		
		LIBSWBF2_LOG_WARN("Could not get field uint32 value at index {0} in DATA chunk '{1}'!", index, GetName());
		return 0;
	}	


	Vector2 Field::GetVector2() const
	{
		Vector2 out;
		if (p_Data->GetVec2(out))
		{
			return out;
		}

		LIBSWBF2_LOG_WARN("Could not get field Vector2 value in DATA chunk '{0}'!", GetName());
		return Vector2();
	}


	Vector3 Field::GetVector3() const
	{
		Vector3 out;
		if (p_Data->GetVec3(out))
		{
			return out;
		}

		LIBSWBF2_LOG_WARN("Could not get field Vector3 value in DATA chunk '{0}'!", GetName());
		return Vector3();
	}


	Vector4 Field::GetVector4() const
	{
		Vector4 out;
		if (p_Data->GetVec4(out))
		{
			return out;
		}

		LIBSWBF2_LOG_WARN("Could not get field Vector4 value in DATA chunk '{0}'!", GetName());
		return Vector4();
	}


	std::string Field::GetString(uint8_t index) const
	{
		std::string strOut;
		if (p_Data->GetString(strOut, index))
		{
			return strOut;
		}
	
		LIBSWBF2_LOG_WARN("Could not get field string value at index {0} in DATA chunk '{1}'!", index, GetName());
		return "";
	}

	std::string Field::GetName() const
	{
		std::string name;
		if (!FNV::Lookup(p_Data->m_NameHash, name))
		{
			name = fmt::format("0x{0:x}", p_Data->m_NameHash);
		}
		return name;
	}


	/*

	Scope

	*/

	void Scope::Cache() const
	{
		if (p_Scope == nullptr)
		{
			return;
		}

		m_Fields = std::move(Field::FieldsFromChunkChildren(p_Scope));
		m_IsValid = true;
	}

	bool Scope::IsEmpty() const
	{
		if (!m_IsValid)
		{
			Cache();
		}
		return m_Fields.size() == 0;
	}


	const Field *Scope::GetField(FNVHash name) const
	{
		if (!m_IsValid)
		{
			Cache();
		}

		for (uint16_t i = 0; i < m_Fields.size(); i++)
		{
			const Field& cur = m_Fields[i];
			if (name == 0 || cur.p_Data->m_NameHash == name)
			{
				return &cur;
			}
		}

		return nullptr;
	}


	std::vector<const Field *> Scope::GetFields(FNVHash name) const
	{
		if (!m_IsValid)
		{
			Cache();
		}

		std::vector<const Field *> matchedFields;
		for (uint16_t i = 0; i < m_Fields.size(); i++)
		{
			const Field& cur = m_Fields[i];
			if (name == 0 || cur.p_Data->m_NameHash == name)
			{
				matchedFields.push_back(&cur);
			}
		}
		return matchedFields;
	}


	Scope::Scope(SCOP *scopePtr)
	{
		p_Scope = scopePtr;
		m_IsValid = false;
	}




	/*

	Config

	*/

	const Field *Config::GetField(FNVHash name) const
	{
		for (uint16_t i = 0; i < m_Fields.size(); i++)
		{
			const Field& cur = m_Fields[i];
			if (name == 0 || cur.p_Data->m_NameHash == name)
			{
				return &cur;
			}
		}
		return nullptr;
	}


	std::vector<const Field *> Config::GetFields(FNVHash name) const
	{
		std::vector<const Field *> matchedFields;
		for (uint16_t i = 0; i < m_Fields.size(); i++)
		{
			const Field& cur = m_Fields[i];
			if (name == 0 || cur.p_Data->m_NameHash == name)
			{
				matchedFields.push_back(&cur);
			}
		}
		return matchedFields;
	}


	bool Config::FromChunk(GenericBaseChunk *cfgPtr, Config& wrapperOut)
	{
		const std::vector<GenericBaseChunk *>& children = cfgPtr -> GetChildren();

		if (children.size() == 0) return false;
		
		config_NAME* nameChunk = dynamic_cast<config_NAME *>(children[0]);
		
		if (nameChunk == nullptr) return false;

		EConfigType type;

		if (dynamic_cast<comb*>(cfgPtr) != nullptr)
		{
			type = EConfigType::Combo;
		}
		else if (dynamic_cast<lght*>(cfgPtr) != nullptr)
		{
			type = EConfigType::Lighting;
		}
		else if (dynamic_cast<path*>(cfgPtr) != nullptr)
		{
			type = EConfigType::Path;
		}
		else if (dynamic_cast<bnd_*>(cfgPtr) != nullptr)
		{
			type = EConfigType::Boundary;
		}
		else if (dynamic_cast<fx__*>(cfgPtr) != nullptr)
		{
			type = EConfigType::Effect;
		}
		else if (dynamic_cast<sky_*>(cfgPtr) != nullptr)
		{
			type = EConfigType::Skydome;
		}
		else if (dynamic_cast<snd_*>(cfgPtr) != nullptr)
		{
			type = EConfigType::Sound;
		}
		else if (dynamic_cast<mus_*>(cfgPtr) != nullptr)
		{
			type = EConfigType::Music;
		}
		else if (dynamic_cast<ffx_*>(cfgPtr) != nullptr)
		{
			type = EConfigType::FoleyFX;
		}
		else if (dynamic_cast<tsr_*>(cfgPtr) != nullptr)
		{
			type = EConfigType::TriggerSoundRegion;
		}
		else if (dynamic_cast<hud_*>(cfgPtr) != nullptr)
		{
			type = EConfigType::HUD;
		}
		else 
		{
			LIBSWBF2_LOG_ERROR("Couldn't wrap unhandled config chunk...");
			return false;
		}

		wrapperOut.m_Type = type; 
		wrapperOut.m_Fields = Field::FieldsFromChunkChildren(cfgPtr);
		wrapperOut.m_Name = nameChunk -> m_Name;

		return true;
	}
}
