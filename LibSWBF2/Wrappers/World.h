#pragma once
#include "req.h"
#include "Instance.h"
#include "Types/Vector3.h"
#include "Types/Vector4.h"
#include "Types/Enums.h"
#include "Types/WorldAnimationKey.h"
#include <string>
#include <vector>

namespace LibSWBF2
{
	class Container;

	namespace Chunks::LVL::wrld
	{
		struct wrld;
		struct regn;
		struct anim;
		struct anmg;
		struct anmh;
		struct BARR;
		struct Hint;
	}
}


namespace LibSWBF2::Wrappers
{
	using Types::Vector3;
	using Types::Vector4;
	using Types::WorldAnimationKey;

	class Level;
	class Instance;
	class Terrain;
	class World;



	class HintNode
	{
	typedef LibSWBF2::Chunks::LVL::wrld::Hint Hint;

		friend World;

		std::shared_ptr<Hint> p_HintNode;
		HintNode() = default;

		static std::optional<HintNode> FromChunk(std::shared_ptr<Hint> chunk);

	public:
		const std::string& GetName() const;
		const Vector3& GetPosition() const;
		Vector4 GetRotation() const;
		const uint16_t& GetType() const;

		void GetProperties(std::vector<FNVHash>& outHashes, std::vector<std::string>& outValues) const;
	};


	class Barrier
	{
	typedef LibSWBF2::Chunks::LVL::wrld::BARR BARR;

		friend World;

		std::shared_ptr<BARR> p_Barrier;
		Barrier() = default;

		static std::optional<Barrier> FromChunk(std::shared_ptr<BARR> chunk);

	public:
		const std::string& GetName() const;
		const Vector3& GetPosition() const;
		Vector4 GetRotation() const;
		const uint32_t& GetFlag() const;
		const Vector3& GetSize() const;		
	};


	class LIBSWBF2_API Region
	{
	typedef LibSWBF2::Chunks::LVL::wrld::regn regn;

		friend World;

		std::shared_ptr<regn> p_Region;
		Region() = default;
		static std::optional<Region> FromChunk(std::shared_ptr<regn> chunk);

	public:
		const std::string& GetName() const;
		const Vector3& GetPosition() const;
		Vector4 GetRotation() const;
		const std::string& GetType() const;
		const Vector3& GetSize() const;

		void GetProperties(std::vector<FNVHash>& outHashes, std::vector<std::string>& outValues) const;
	};

	
	class LIBSWBF2_API WorldAnimation
	{
	typedef LibSWBF2::Chunks::LVL::wrld::anim anim;

		friend World;

		std::shared_ptr<anim> p_WorldAnimation;
		WorldAnimation() = default;
		static std::optional<WorldAnimation> FromChunk(std::shared_ptr<anim> chunk);

	public:
		const std::string& GetName() const;
		float GetRunTime() const;
		bool IsLooping() const;
		bool IsTranslationLocal() const;
		std::vector<WorldAnimationKey> GetRotationKeys() const;
		std::vector<WorldAnimationKey> GetPositionKeys() const;
	};


	class LIBSWBF2_API WorldAnimationGroup
	{
	typedef LibSWBF2::Chunks::LVL::wrld::anmg anmg;

		friend World;

		std::shared_ptr<anmg> p_WorldAnimationGroup;
		WorldAnimationGroup() = default;
		static std::optional<WorldAnimationGroup> FromChunk(std::shared_ptr<anmg> chunk);

	public:
		const std::string& GetName() const;
		bool IsPlayingAtStart() const;
		bool IsStoppedOnControl() const;
		bool DisablesHierarchies() const;
		void GetAnimationInstancePairs(std::vector<std::string>& animNamesOut, std::vector<std::string>& instanceNamesOut) const;
	};


	class LIBSWBF2_API WorldAnimationHierarchy
	{
	typedef LibSWBF2::Chunks::LVL::wrld::anmh anmh;

		friend World;

		std::shared_ptr<anmh> p_WorldAnimationHierarchy;
		WorldAnimationHierarchy() = default;
		static std::optional<WorldAnimationHierarchy> FromChunk(std::shared_ptr<anmh> chunk);

	public:
		const std::string& GetRootName() const;
		const std::vector<std::string>& GetChildNames() const;
	};


	class LIBSWBF2_API World
	{
	typedef LibSWBF2::Chunks::LVL::wrld::wrld wrld;

	private:
		friend Level;

		std::weak_ptr<Container> m_MainContainer;

		std::vector<Instance> m_Instances;
		std::vector<Region> m_Regions;
		std::vector<Barrier> m_Barriers;
		std::vector<HintNode> m_HintNodes;
		std::vector<WorldAnimation> m_Animations;
		std::vector<WorldAnimationGroup> m_AnimationGroups;
		std::vector<WorldAnimationHierarchy> m_AnimationHierarchies;

		std::shared_ptr<wrld> p_World;


	public:
		static std::optional<World> FromChunk(std::shared_ptr<Container> mainContainer, std::shared_ptr<wrld> worldChunk);

		std::string GetName() const;
		std::vector<Instance> GetInstances() const;
		const std::vector<Region>& GetRegions() const;
		const std::vector<Barrier>& GetBarriers() const;
		const std::vector<HintNode>& GetHintNodes() const;
		std::string GetTerrainName() const;
		const Terrain* GetTerrain() const;
		std::string GetSkyName() const;

		const std::vector<WorldAnimation>& GetAnimations() const;
		const std::vector<WorldAnimationGroup>& GetAnimationGroups() const;
		const std::vector<WorldAnimationHierarchy>& GetAnimationHierarchies() const;
	};
}
