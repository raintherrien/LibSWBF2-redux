#pragma once
#include "req.h"
#include "EntityClass.h"
#include "Types/Vector4.h"
#include "Chunks/LVL/wrld/inst.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace LibSWBF2
{
	class Container;
}

namespace LibSWBF2::Wrappers
{
	using namespace LibSWBF2::Chunks::LVL::wrld;
	using Types::Vector3;
	using Types::Vector4;

	class Level;
	class Model;
	class World;

	class LIBSWBF2_API Instance
	{
	private:
		friend World;

	private:
		Container* p_MainContainer;
		inst* p_Instance;
		std::unordered_map<FNVHash, std::vector<uint32_t>> m_HashToIndices;

	public:
		static bool FromChunk(Container* mainContainer, inst* instanceChunk, Instance& out);

		const std::string& GetEntityClassName() const;
		const std::string& GetName() const;
		Vector3 GetPosition() const;
		Vector4 GetRotation() const;

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

		void GetOverriddenProperties(std::vector<FNVHash>& hashesOut, std::vector<std::string>& valuesOut) const;
		const EntityClass* GetEntityClass() const;
	};
}
