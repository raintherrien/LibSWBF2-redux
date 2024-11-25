#pragma once
#include "Types/Enums.h"
#include <string>
#include <vector>

namespace LibSWBF2
{
	class Container;

	namespace Chunks::LVL::common
	{
		struct GenericClassNC;
	}
}

namespace LibSWBF2::Wrappers
{
	class LIBSWBF2_API EntityClass
	{
		typedef LibSWBF2::Chunks::LVL::common::GenericClassNC GenericClassNC;
	private:
		friend class Level;

		EntityClass& operator=(const EntityClass& other);
		EntityClass& operator=(EntityClass&& other);

	private:
		Container* p_MainContainer;
		GenericClassNC* p_classChunk;
		EEntityClassType m_EntityClassType;
		class PropertyMap* m_PropertyMapping;

	public:
		EntityClass();
		~EntityClass();
		EntityClass(const EntityClass &);

		// Only available template instances for "FromChunk" are:
		// - entc*
		// - ordc*
		// - wpnc*
		// - expc*
		template<class ChunkType>
		static bool FromChunk(Container* mainContainer, ChunkType* classChunk, EntityClass& out);

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
	};
}
