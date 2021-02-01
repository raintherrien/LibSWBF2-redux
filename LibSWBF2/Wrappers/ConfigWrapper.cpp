#include "req.h"
#include "stdafx.h"
#include "ConfigWrapper.h"
#include "Chunks/LVL/config/ConfigChunk.h"
#include "Types/Color4.h"
#include "Types/Vector4.h"
#include "Types/Vector3.h"


namespace LibSWBF2::Wrappers
{
	using namespace LibSWBF2::Chunks::LVL::config;
	using namespace LibSWBF2::Types;


	List<DATA_CONFIG *> Config::SearchPropertyAll(FNVHash hash) const
	{
		List<DATA_CONFIG *> matches;
		const List<GenericBaseChunk*>& children = p_Chunk -> GetChildren();

		for (int i = 0; i < children.Size(); i++)
		{
			DATA_CONFIG *childDATA = dynamic_cast<DATA_CONFIG *>(children[i]);

			if (childDATA != nullptr && childDATA -> m_NameHash == hash)
			{
 				matches.Add(childDATA);	
			}
		}

		return matches;
	}


	DATA_CONFIG *Config::SearchProperty(FNVHash hash) const
	{
		const List<GenericBaseChunk*>& children = p_Chunk -> GetChildren();

		for (int i = 0; i < children.Size(); i++)
		{
			DATA_CONFIG *childDATA = dynamic_cast<DATA_CONFIG *>(children[i]);

			if (childDATA != nullptr && childDATA -> m_NameHash == hash)
			{
 				return childDATA;
			}
		}

		return nullptr;
	}


	/*
	FNVHash Config::GetName()
	{
		const List<GenericBaseChunk*>& children = p_Chunk -> GetChildren();

		if (children.Size() > 0)
		{
			config_NAME *nameChunk = dynamic_cast<config_NAME *>(children[0]); 

			if (config_NAME != nullptr)
			{
				return config_NAME -> m_PropertyName;
			}
		}

		return 0;
	}
	*/


	bool Config::IsPropertySet(FNVHash hash) const
	{
		return SearchProperty(hash) != nullptr;
	}


	float_t Config::GetFloat(FNVHash hash, uint32_t index) const
	{
		DATA_CONFIG *propertyChunk = SearchProperty(hash);
		if (propertyChunk != nullptr)
		{
			float_t out;
			if (propertyChunk -> GetFloat(out,index))
			{
				return out;
			}
		}

		return 0.0f;
	}
		
	Vector2 Config::GetVector2(FNVHash hash) const
	{
		DATA_CONFIG *propertyChunk = SearchProperty(hash);
		if (propertyChunk != nullptr)
		{
			Vector2 out;
			if (propertyChunk -> GetVec2(out))
			{
				return out;
			}
		}

		return Vector2();
	}


	Vector3 Config::GetVector3(FNVHash hash) const
	{
		DATA_CONFIG *propertyChunk = SearchProperty(hash);
		if (propertyChunk != nullptr)
		{
			Vector3 out;
			if (propertyChunk -> GetVec3(out))
			{
				return out;
			}
		}

		return Vector3();
	}


	Vector4 Config::GetVector4(FNVHash hash) const
	{
		DATA_CONFIG *propertyChunk = SearchProperty(hash);
		if (propertyChunk != nullptr)
		{
			Vector4 out;
			if (propertyChunk -> GetVec4(out))
			{
				return out;
			}
		}

		return Vector4();
	}


	String Config::GetString(FNVHash hash) const
	{
		DATA_CONFIG *propertyChunk = SearchProperty(hash);
		if (propertyChunk != nullptr)
		{
			String strOut;
			if (propertyChunk -> GetString(strOut))
			{
				return strOut;
			}
		}

		return "";
	}


	List<String> Config::GetStrings(FNVHash hash) const
	{
		List<String> stringsOut;
		List<DATA_CONFIG *> propertyChunks = SearchPropertyAll(hash);
		for (int i = 0; i < propertyChunks.Size(); i++)
		{
			DATA_CONFIG *propertyChunk = propertyChunks[i];
			String strOut;
			if (propertyChunk -> GetString(strOut))
			{
				stringsOut.Add(strOut);
			}
		}

		return stringsOut;
	}



	List<Config> Config::GetChildConfigs(FNVHash hash) const
	{
		List<Config> foundSCOPs;

		const List<GenericBaseChunk*>& children = p_Chunk -> GetChildren();

		if (children.Size() < 2)
		{
			return foundSCOPs;
		}

		for (int i = 0; i < children.Size() - 1; i++)
		{
			DATA_CONFIG *data = dynamic_cast<DATA_CONFIG *>(children[i]);

			if (data == nullptr || data -> m_NameHash != hash)
			{
				continue;
			}

			SCOP *childSCOP = dynamic_cast<SCOP *>(children[i+1]);

			if (childSCOP != nullptr)
			{
 				foundSCOPs.Add(Config(childSCOP));
			}
		}

		return foundSCOPs;		
	}


	bool Config::FromChunk(GenericBaseChunk *cfgPtr, Config& wrapperOut)
	{
		const List<GenericBaseChunk *>& children = cfgPtr -> GetChildren();

		if (children.Size() == 0) return false;
		
		config_NAME* nameChunk = dynamic_cast<config_NAME *>(children[0]);
		
		if (nameChunk == nullptr) return false;

		Config cfg;

		cfg.m_Name = nameChunk -> m_PropertyName;
		cfg.p_Chunk = cfgPtr;

		wrapperOut = cfg;

		return true;
	}


	Config::Config(SCOP *scop)
	{
		p_Chunk = scop;
	}

}


