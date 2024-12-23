#include "pch.h"
#include "EntityClass.h"
#include "Hashing.h"
#include "Container.h"

#include <string>
#include <map>


typedef LibSWBF2::Chunks::LVL::common::PROP PROP;

namespace LibSWBF2::Wrappers
{
	EEntityClassType EntityClass::GetClassType() const
	{
		return m_EntityClassType;
	}

	std::string EntityClass::GetTypeName() const
	{
		if (p_classChunk == nullptr) {
			LIBSWBF2_THROW("Invalid class chunk");
		}
		if (p_classChunk->p_Type == nullptr) {
			LIBSWBF2_THROW("Invalid class chunk type");
		}
		return p_classChunk->p_Type->m_Text;
	}

	std::string EntityClass::GetBaseName() const
	{
		if (p_classChunk == nullptr) {
			LIBSWBF2_THROW("Invalid class chunk");
		}
		if (p_classChunk->p_Base == nullptr) {
			LIBSWBF2_THROW("Invalid class chunk base");
		}
		return p_classChunk->p_Base->m_Text;
	}

	const EntityClass* EntityClass::GetBase() const
	{
		if (auto container = p_MainContainer.lock())
		{
			return container->FindEntityClass(GetBaseName());
		}
		return nullptr;
	}

	bool EntityClass::GetProperty(FNVHash hashedPropertyName, std::string& outValue) const
	{
		auto it = m_HashToIndices.find(hashedPropertyName);
		if (it != m_HashToIndices.end() && it->second.size() > 0)
		{
			outValue = p_classChunk->m_Properties[it->second[0]]->m_Value;
			return true;
		}
		const EntityClass* base = GetBase();
		if (base != nullptr)
		{
			return base->GetProperty(hashedPropertyName, outValue);
		}
		return false;
	}

	bool EntityClass::GetProperty(const std::string& propertyName, std::string& outValue) const
	{
		if (propertyName.empty())
		{
			return false;
		}
		return GetProperty(FNV::Hash(propertyName), outValue);
	}

	bool EntityClass::GetProperty(const std::string& propertyName, std::vector<std::string>& outValues) const
	{
		if (propertyName.empty())
		{
			return false;
		}
		return GetProperty(FNV::Hash(propertyName), outValues);
	}

	bool EntityClass::GetProperty(FNVHash hashedPropertyName, std::vector<std::string>& outValues) const
	{
		auto it = m_HashToIndices.find(hashedPropertyName);
		if (it != m_HashToIndices.end() && it->second.size() > 0)
		{
			for (size_t i = 0; i < it->second.size(); ++i)
			{
				outValues.push_back(p_classChunk->m_Properties[it->second[i]]->m_Value);
			}

			// For multi properties (e.g. ControlSpeed) also crawl down
			// all the base classes for that same property name.
			// So we actually do NOT return here.
		}

		const EntityClass* base = GetBase();
		if (base != nullptr)
		{
			base->GetProperty(hashedPropertyName, outValues);
		}
		return outValues.size() > 0;
	}

	void EntityClass::GetOverriddenProperties(std::vector<FNVHash>& outHashes, std::vector<std::string>& outValues) const
	{
		outHashes.clear();
		outValues.clear();

		std::vector<std::shared_ptr<PROP>>& properties = p_classChunk -> m_Properties;
		for (int i = 0; i < properties.size(); i++)
		{
			outHashes.push_back(properties[i] -> m_PropertyName);
			outValues.push_back(properties[i] -> m_Value);
		}
	}

	std::vector<FNVHash> EntityClass::GetAllPropertyHashes() const
	{
		std::vector<FNVHash> outHashes;

		const EntityClass* base = GetBase();
		if (base != nullptr)
		{
			std::vector<FNVHash> base_hashes = base->GetAllPropertyHashes();
			outHashes.insert(outHashes.end(), base_hashes.begin(), base_hashes.end());
		}

		std::vector<std::shared_ptr<PROP>>& properties = p_classChunk->m_Properties;
		for (int i = 0; i < properties.size(); i++)
		{
			outHashes.push_back(properties[i]->m_PropertyName);
		}

		return outHashes;
	}

	void EntityClass::GetAllProperties(std::vector<FNVHash>& outHashes, std::vector<std::string>& outValues) const
	{
		outHashes.clear();
		outValues.clear();

		const EntityClass* base = GetBase();
		if (base != nullptr)
		{
			base->GetAllProperties(outHashes, outValues);
		}

		std::vector<std::shared_ptr<PROP>>& properties = p_classChunk->m_Properties;
		for (int i = 0; i < properties.size(); i++)
		{
			outHashes.push_back(properties[i]->m_PropertyName);
			outValues.push_back(properties[i]->m_Value);
		}
	}
}
