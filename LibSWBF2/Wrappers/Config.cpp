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

	std::vector<Field> Field::FieldsFromChunkChildren(std::shared_ptr<GenericBaseChunk> chunk)
	{
		std::vector<Field> fields;
		const std::vector<std::shared_ptr<GenericBaseChunk>>& children = chunk -> GetChildren();
		
		for (uint16_t i = 0; i < children.size(); i++)
		{
			std::shared_ptr<DATA_CONFIG> child = std::dynamic_pointer_cast<DATA_CONFIG>(children[i]);
			
			if (!child) {
				continue;
			}
			
			std::shared_ptr<SCOP> scope;
			if (i != children.size() - 1)
			{
				scope = std::dynamic_pointer_cast<SCOP>(children[i+1]);
			}
				
			fields.push_back(Field(child, scope));
		}

		return fields;
	}


	Field::Field(std::shared_ptr<DATA_CONFIG> data, std::shared_ptr<SCOP> scop)
		: m_Scope(scop), p_Data{data}
	{
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

		m_Fields = Field::FieldsFromChunkChildren(p_Scope);
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


	Scope::Scope(std::shared_ptr<SCOP> scopePtr)
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


	std::optional<Config> Config::FromChunk(std::shared_ptr<GenericBaseChunk> cfgPtr)
	{
		const std::vector<std::shared_ptr<GenericBaseChunk>>& children = cfgPtr -> GetChildren();

		if (children.size() == 0) {
			return {};
		}
		
		std::shared_ptr<config_NAME> nameChunk = std::dynamic_pointer_cast<config_NAME>(children[0]);
		
		if (!nameChunk) {
			return {};
		}

		EConfigType type;

		if (std::dynamic_pointer_cast<comb>(cfgPtr))
		{
			type = EConfigType::Combo;
		}
		else if (std::dynamic_pointer_cast<lght>(cfgPtr))
		{
			type = EConfigType::Lighting;
		}
		else if (std::dynamic_pointer_cast<path>(cfgPtr))
		{
			type = EConfigType::Path;
		}
		else if (std::dynamic_pointer_cast<bnd_>(cfgPtr))
		{
			type = EConfigType::Boundary;
		}
		else if (std::dynamic_pointer_cast<fx__>(cfgPtr))
		{
			type = EConfigType::Effect;
		}
		else if (std::dynamic_pointer_cast<sky_>(cfgPtr))
		{
			type = EConfigType::Skydome;
		}
		else if (std::dynamic_pointer_cast<snd_>(cfgPtr))
		{
			type = EConfigType::Sound;
		}
		else if (std::dynamic_pointer_cast<mus_>(cfgPtr))
		{
			type = EConfigType::Music;
		}
		else if (std::dynamic_pointer_cast<ffx_>(cfgPtr))
		{
			type = EConfigType::FoleyFX;
		}
		else if (std::dynamic_pointer_cast<tsr_>(cfgPtr))
		{
			type = EConfigType::TriggerSoundRegion;
		}
		else if (std::dynamic_pointer_cast<hud_>(cfgPtr))
		{
			type = EConfigType::HUD;
		}
		else 
		{
			LIBSWBF2_LOG_ERROR("Couldn't wrap unhandled config chunk...");
			return {};
		}

		Config wrapperOut;

		wrapperOut.m_Type = type; 
		wrapperOut.m_Fields = Field::FieldsFromChunkChildren(cfgPtr);
		wrapperOut.m_Name = nameChunk -> m_Name;

		return wrapperOut;
	}
}
