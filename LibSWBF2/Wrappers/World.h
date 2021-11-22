#pragma once
#include "req.h"

#include "Types/LibString.h"
#include "Types/List.h"
#include "Types/Vector3.h"
#include "Types/Vector4.h"
#include "Types/Enums.h"
#include "Types/WorldAnimationKey.h"


namespace LibSWBF2
{
	class Container;

	namespace Chunks::LVL::wrld
	{
		struct wrld;
		struct regn;
		struct anim;
	}
}


namespace LibSWBF2::Wrappers
{
	using Types::List;
	using Types::String;
	using Types::Vector3;
	using Types::Vector4;
	using Types::WorldAnimationKey;

	class Level;
	class Instance;
	class Terrain;
	class World;


	class LIBSWBF2_API Region
	{
	typedef LibSWBF2::Chunks::LVL::wrld::regn regn;

		friend World;
		friend List<Region>;

		regn* p_Region;
		Region() = default;
		static bool FromChunk(regn* chunk, Region& regOut);

	public:
		const String& GetName() const;
		const Vector3& GetPosition() const;
		Vector4 GetRotation() const;
		const String& GetType() const;
		const Vector3& GetSize() const;
	};

	
	class LIBSWBF2_API WorldAnimation
	{
	typedef LibSWBF2::Chunks::LVL::wrld::anim anim;

		friend World;
		friend List<WorldAnimation>;

		anim* p_WorldAnimation;
		WorldAnimation() = default;
		static bool FromChunk(anim* chunk, WorldAnimation& animOut);		

	public:
		const String& GetName() const;
		const float GetRunTime() const;
		const bool IsLooping() const;
		const bool IsTranslationLocal() const;
		List<WorldAnimationKey> GetRotationKeys() const;
		List<WorldAnimationKey> GetPositionKeys() const;
	};



	class LIBSWBF2_API World
	{
	typedef LibSWBF2::Chunks::LVL::wrld::wrld wrld;

	private:
		friend Level;
		friend List<World>;

		World() = default;
		~World() = default;

		Container* m_MainContainer = nullptr;

		List<Instance> m_Instances;
		List<Region> m_Regions;
		List<WorldAnimation> m_Animations;

		wrld* p_World;


	public:
		static bool FromChunk(Container* mainContainer, wrld* worldChunk, World& out);

		const String& GetName() const;
		const List<Instance>& GetInstances() const;
		const List<Region>& GetRegions() const;
		Types::String GetTerrainName() const;
		const Terrain* GetTerrain() const;
		Types::String GetSkyName() const;


		const List<WorldAnimation>& GetAnimations() const;

		// Returns names of anim groups present
		const List<String> GetAnimationGroups() const;

		// Anim groups consist of pairs between instances and animations
		// Returns false if <animGroupName> not present
		const bool GetAnimationGroupPairs(const String& animGroupName, 
										List<String>& animNamesOut, 
										List<String>& instanceNamesOut) const;
	};
}