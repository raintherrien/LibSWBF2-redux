#pragma once
#include "Types/Enums.h"
#include "Chunks/LVL/common/GenericClass.h"
#include "InternalHelpers.h"
#include <string>
#include <vector>

namespace LibSWBF2
{
	class Container;

	namespace Chunks::LVL::common
	{
		struct GenericClass;
	}
}

namespace LibSWBF2::Wrappers
{
	class LIBSWBF2_API EntityClass
	{
		typedef LibSWBF2::Chunks::LVL::common::GenericClass GenericClass;
	private:
		friend class Level;

	private:
		std::weak_ptr<Container> p_MainContainer;
		std::shared_ptr<GenericClass> p_classChunk;
		EEntityClassType m_EntityClassType;
		std::unordered_map<FNVHash, std::vector<uint32_t>> m_HashToIndices;

	public:
		EEntityClassType GetClassType() const;
		std::string GetTypeName() const;
		std::string GetBaseName() const;
		const EntityClass* GetBase() const;

		// Note about properties:
		// Property names are not unique! There's potentially more than one
		// property under one name. Example from com_bldg_controlzone:
		//   HoloImageGeometry = "com_icon_alliance alliance"
		//   HoloImageGeometry = "com_icon_imperial empire"
		//   HoloImageGeometry = "com_icon_republic republic"
		//   HoloImageGeometry = "com_icon_CIS CIS"

		// these will return the first encounter. will fall back to base class, if existent
		bool GetProperty(FNVHash hashedPropertyName, std::string& outValue) const;
		bool GetProperty(const std::string& propertyName, std::string& outValue) const;

		// these will return all encounters. will also recursively search base classes, if existent
		bool GetProperty(const std::string& propertyName, std::vector<std::string>& outValues) const;
		bool GetProperty(FNVHash hashedPropertyName, std::vector<std::string>& outValues) const;

		void GetOverriddenProperties(std::vector<FNVHash>& outHashes, std::vector<std::string>& outValues) const;
		std::vector<FNVHash> GetAllPropertyHashes() const;
		void GetAllProperties(std::vector<FNVHash>& outHashes, std::vector<std::string>& outValues) const;

		template<class ChunkType>
		static std::optional<EntityClass> FromChunk(std::shared_ptr<Container> mainContainer, std::shared_ptr<ChunkType> classChunk)
		{
			using LibSWBF2::Chunks::LVL::common::entc;
			using LibSWBF2::Chunks::LVL::common::ordc;
			using LibSWBF2::Chunks::LVL::common::wpnc;
			using LibSWBF2::Chunks::LVL::common::expc;

			EntityClass out;

			if (classChunk == nullptr)
			{
				LIBSWBF2_LOG_ERROR("Given classChunk was NULL!");
				return {};
			}

			out.p_classChunk = std::dynamic_pointer_cast<GenericClass>(classChunk);
			if (out.p_classChunk == nullptr) {
				LIBSWBF2_THROW("Invalid class chunk");
			}
			out.p_MainContainer = mainContainer;

			if (typeid(ChunkType*) == typeid(entc*))
			{
				out.m_EntityClassType = EEntityClassType::GameObjectClass;
			}
			else if (typeid(ChunkType*) == typeid(ordc*))
			{
				out.m_EntityClassType = EEntityClassType::OrdnanceClass;
			}
			else if (typeid(ChunkType*) == typeid(wpnc*))
			{
				out.m_EntityClassType = EEntityClassType::WeaponClass;
			}
			else if (typeid(ChunkType*) == typeid(expc*))
			{
				out.m_EntityClassType = EEntityClassType::ExplosionClass;
			}
			else
			{
				LIBSWBF2_LOG_ERROR("Invalid EntityClass Type: {}", typeid(classChunk).name());
				return {};
			}

			out.m_HashToIndices.clear();
			for (size_t i = 0; i < classChunk->m_Properties.size(); ++i)
			{
				FNVHash hashedName = classChunk->m_Properties[i]->m_PropertyName;

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

			return out;
		}
	};
}
