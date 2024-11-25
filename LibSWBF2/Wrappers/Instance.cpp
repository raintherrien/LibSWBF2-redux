#include "pch.h"
#include "Instance.h"
#include "InternalHelpers.h"
#include "Hashing.h"
#include "Level.h"
#include "Container.h"

namespace LibSWBF2::Wrappers
{
	bool Instance::FromChunk(Container* mainContainer, inst* instanceChunk, Instance& out)
	{
		if (instanceChunk == nullptr)
		{
			LOG_ERROR("Given instanceChunk was NULL!");
			return false;
		}

		out.p_MainContainer = mainContainer;
		out.p_Instance = instanceChunk;

		out.m_HashToIndices.clear();
		for (size_t i = 0; i < instanceChunk->m_OverrideProperties.size(); ++i)
		{
			FNVHash hashedName = instanceChunk->m_OverrideProperties[i]->m_PropertyName;

			auto it = out.m_HashToIndices.find(hashedName);
			if (it != out.m_HashToIndices.end())
			{
				out.m_HashToIndices[hashedName].push_back((uint32_t)i);
			}
			else
			{
				out.m_HashToIndices.insert(std::make_pair(hashedName, std::vector<uint32_t>{ (uint32_t)i }));
			}
		}

		return true;
	}

	std::string Instance::GetEntityClassName() const
	{
		return p_Instance->p_Info->p_Type->m_Text;
	}

	std::string Instance::GetName() const
	{
		return p_Instance->p_Info->p_Name->m_Text;
	}

	Vector3 Instance::GetPosition() const
	{
		return p_Instance->p_Info->p_XFRM->m_Position;
	}

	Vector4 Instance::GetRotation() const
	{
		return MatrixToQuaternion(p_Instance->p_Info->p_XFRM->m_RotationMatrix);
	}

	const EntityClass* Instance::GetEntityClass() const
	{
		if (p_MainContainer == nullptr)
		{
			return nullptr;
		}
		return p_MainContainer->FindEntityClass(GetEntityClassName());
	}

	bool Instance::GetProperty(FNVHash hashedPropertyName, std::string& outValue) const
	{
		auto it = m_HashToIndices.find(hashedPropertyName);
		if (it != m_HashToIndices.end() && it->second.size() > 0)
		{
			outValue = p_Instance->m_OverrideProperties[it->second[0]]->m_Value;
			return true;
		}
		
		const EntityClass* entityClass = GetEntityClass();
		if (entityClass == nullptr)
		{
			// This can happen if the Entity Class is in another LVL (e.g. com_bldg_controlzone)
			// and there's no Container above the currently owning Level instance
			return false;
		}

		return entityClass->GetProperty(hashedPropertyName, outValue);
	}

	bool Instance::GetProperty(const std::string& propertyName, std::string& outValue) const
	{
		if (propertyName.empty())
		{
			return false;
		}
		return GetProperty(FNV::Hash(propertyName), outValue);
	}

	bool Instance::GetProperty(const std::string& propertyName, std::vector<std::string>& outValues) const
	{
		if (propertyName.empty())
		{
			return false;
		}
		return GetProperty(FNV::Hash(propertyName), outValues);
	}

	bool Instance::GetProperty(FNVHash hashedPropertyName, std::vector<std::string>& outValues) const
	{
		outValues.clear();
		auto it = m_HashToIndices.find(hashedPropertyName);
		if (it != m_HashToIndices.end() && it->second.size() > 0)
		{
			for (size_t i = 0; i < it->second.size(); ++i)
			{
				outValues.push_back(p_Instance->m_OverrideProperties[it->second[i]]->m_Value);
			}
			return true;
		}
		const EntityClass* instanceClass = GetEntityClass();
		if (instanceClass != nullptr)
		{
			return instanceClass->GetProperty(hashedPropertyName, outValues);
		}
		return false;
	}

	void Instance::GetOverriddenProperties(std::vector<FNVHash>& hashesOut, std::vector<std::string>& valuesOut) const
	{
		hashesOut.clear();
		valuesOut.clear();

		std::vector<PROP*>& properties = p_Instance -> m_OverrideProperties;
		for (int i = 0; i < properties.size(); i++)
		{
			hashesOut.push_back(properties[i] -> m_PropertyName);
			valuesOut.push_back(properties[i] -> m_Value);
		}
	}
}
