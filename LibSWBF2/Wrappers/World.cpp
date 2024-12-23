#include "pch.h"
#include "World.h"
#include "Instance.h"
#include "Terrain.h"
#include "InternalHelpers.h"
#include "Container.h"

#include "Chunks/LVL/wrld/wrld.h"
#include "Chunks/LVL/wrld/anmg.INFO.h"
#include "Chunks/LVL/wrld/anmh.INFO.h"


namespace LibSWBF2::Wrappers
{

	// HintNode

	std::optional<HintNode> HintNode::FromChunk(std::shared_ptr<Hint> chunk)
	{
		HintNode hintOut;

		if (chunk -> p_Info == nullptr ||
			chunk -> p_Info -> p_Name == nullptr ||
			chunk -> p_Info -> p_Type == nullptr ||
			chunk -> p_Info -> p_Transform == nullptr)
		{
			return {};
		}

		hintOut.p_HintNode = chunk;
		return hintOut;
	}

	const std::string& HintNode::GetName() const
	{
		return p_HintNode -> p_Info -> p_Name -> m_Text;
	}

	const Vector3& HintNode::GetPosition() const
	{
		return p_HintNode -> p_Info -> p_Transform -> m_Position;
	}

	Vector4 HintNode::GetRotation() const
	{
		return MatrixToQuaternion(p_HintNode -> p_Info -> p_Transform -> m_RotationMatrix);
	}

	const uint16_t& HintNode::GetType() const
	{
		return p_HintNode -> p_Info -> p_Type -> m_Type;
	}

	void HintNode::GetProperties(std::vector<FNVHash>& outHashes, std::vector<std::string>& outValues) const
	{
		outHashes.clear();
		outValues.clear();

		std::vector<std::shared_ptr<PROP>>& properties = p_HintNode->m_Properties;
		for (int i = 0; i < properties.size(); i++)
		{
			outHashes.push_back(properties[i]->m_PropertyName);
			outValues.push_back(properties[i]->m_Value);
		}
	}





	// Barrier

	std::optional<Barrier> Barrier::FromChunk(std::shared_ptr<BARR> chunk)
	{
		Barrier barrOut;

		if (chunk -> p_Info == nullptr ||
			chunk -> p_Info -> p_Name == nullptr ||
			chunk -> p_Info -> p_Flag == nullptr ||
			chunk -> p_Info -> p_Size == nullptr ||
			chunk -> p_Info -> p_Transform == nullptr)
		{
			return {};
		}

		barrOut.p_Barrier = chunk;
		return barrOut;
	}

	const std::string& Barrier::GetName() const
	{
		return p_Barrier -> p_Info -> p_Name -> m_Text;
	}

	const Vector3& Barrier::GetPosition() const
	{
		return p_Barrier -> p_Info -> p_Transform -> m_Position;
	}

	Vector4 Barrier::GetRotation() const
	{
		return MatrixToQuaternion(p_Barrier -> p_Info -> p_Transform -> m_RotationMatrix);
	}

	const Vector3& Barrier::GetSize() const
	{
		return p_Barrier -> p_Info -> p_Size -> m_Dimensions;
	}

	const uint32_t& Barrier::GetFlag() const
	{
		return p_Barrier -> p_Info -> p_Flag -> m_Flag;
	}



	// Region

	std::optional<Region> Region::FromChunk(std::shared_ptr<regn> chunk)
	{
		Region regOut;

		if (chunk -> p_Info == nullptr ||
			chunk -> p_Info -> p_Name == nullptr ||
			chunk -> p_Info -> p_Type == nullptr ||
			chunk -> p_Info -> p_SIZE == nullptr ||
			chunk -> p_Info -> p_XFRM == nullptr)
		{
			return {};
		}

		regOut.p_Region = chunk;
		return regOut;
	}

	const std::string& Region::GetName() const
	{
		return p_Region -> p_Info -> p_Name -> m_Text;
	}

	const Vector3& Region::GetPosition() const
	{
		return p_Region -> p_Info -> p_XFRM -> m_Position;
	}

	Vector4 Region::GetRotation() const
	{
		return MatrixToQuaternion(p_Region -> p_Info -> p_XFRM -> m_RotationMatrix);
	}

	const Vector3& Region::GetSize() const
	{
		return p_Region -> p_Info -> p_SIZE -> m_Dimensions;
	}

	const std::string& Region::GetType() const
	{
		return p_Region -> p_Info -> p_Type -> m_Text;
	}

	void Region::GetProperties(std::vector<FNVHash>& outHashes, std::vector<std::string>& outValues) const
	{
		outHashes.clear();
		outValues.clear();

		std::vector<std::shared_ptr<PROP>>& properties = p_Region->m_Props;
		for (int i = 0; i < properties.size(); i++)
		{
			outHashes.push_back(properties[i]->m_PropertyName);
			outValues.push_back(properties[i]->m_Value);
		}
	}



	// World Animation

	std::optional<WorldAnimation> WorldAnimation::FromChunk(std::shared_ptr<anim> chunk)
	{
		WorldAnimation animOut;

		if (chunk -> p_Info == nullptr)
		{
			return {};
		}

		animOut.p_WorldAnimation = chunk;
		return animOut;
	}

	const std::string& WorldAnimation::GetName() const
	{
		return p_WorldAnimation -> p_Info -> m_Name;
	}

	float WorldAnimation::GetRunTime() const
	{
		return p_WorldAnimation -> p_Info -> m_RunTime;
	}


	bool WorldAnimation::IsLooping() const
	{
		return p_WorldAnimation -> p_Info -> m_Looping == 1;
	}

	bool WorldAnimation::IsTranslationLocal() const
	{
		return p_WorldAnimation -> p_Info -> m_LocalTranslation == 1;
	}

	std::vector<WorldAnimationKey> WorldAnimation::GetRotationKeys() const
	{
		std::vector<WorldAnimationKey> AnimKeys;
		for (int i = 0; i < p_WorldAnimation -> m_RotationKeys.size(); i++)
		{
			std::shared_ptr<ROTK> currKey = p_WorldAnimation -> m_RotationKeys[i];
			AnimKeys.push_back(currKey -> m_Key);
		}	
		return AnimKeys;	
	}
	
	std::vector<WorldAnimationKey> WorldAnimation::GetPositionKeys() const
	{
		std::vector<WorldAnimationKey> AnimKeys;
		for (int i = 0; i < p_WorldAnimation -> m_PositionKeys.size(); i++)
		{
			std::shared_ptr<POSK> currKey = p_WorldAnimation -> m_PositionKeys[i];
			AnimKeys.push_back(currKey -> m_Key);
		}		
		return AnimKeys;
	}



	// World Animation Group

	std::optional<WorldAnimationGroup> WorldAnimationGroup::FromChunk(std::shared_ptr<anmg> chunk)
	{
		WorldAnimationGroup groupOut;

		if (chunk -> p_Info == nullptr)
		{
			return {};
		}

		groupOut.p_WorldAnimationGroup = chunk;
		return groupOut;
	}

	const std::string& WorldAnimationGroup::GetName() const
	{
		return p_WorldAnimationGroup -> p_Info -> m_Name;	
	}

	bool WorldAnimationGroup::IsPlayingAtStart() const
	{
		return p_WorldAnimationGroup -> p_Info -> m_PlayAtStart == 1;
	}

	bool WorldAnimationGroup::IsStoppedOnControl() const
	{
		return p_WorldAnimationGroup -> p_Info -> m_StopOnControl == 1;
	}

	bool WorldAnimationGroup::DisablesHierarchies() const
	{
		return p_WorldAnimationGroup -> p_NoHierarchy != nullptr;
	}

	void WorldAnimationGroup::GetAnimationInstancePairs(
									std::vector<std::string>& animNamesOut, 
									std::vector<std::string>& instanceNamesOut) const
	{
		animNamesOut.clear();
		instanceNamesOut.clear();

		for (uint16_t j = 0; j < p_WorldAnimationGroup -> m_AnimObjectPairs.size(); j++)
		{
			auto& pair = p_WorldAnimationGroup -> m_AnimObjectPairs[j] -> m_Texts;
			if (pair.size() != 2)
			{
				continue;
			}

			animNamesOut.push_back(pair[0]);
			instanceNamesOut.push_back(pair[1]);
		}
	}



	// World Animation Hierarchy

	std::optional<WorldAnimationHierarchy> WorldAnimationHierarchy::FromChunk(std::shared_ptr<anmh> chunk)
	{
		WorldAnimationHierarchy hierOut;
		if (chunk -> p_Info == nullptr || chunk -> p_Info -> m_NumStrings == 0)
		{
			return {};
		}

		hierOut.p_WorldAnimationHierarchy = chunk;
		return hierOut;
	}

	const std::string& WorldAnimationHierarchy::GetRootName() const
	{
		return p_WorldAnimationHierarchy -> p_Info -> m_RootName;
	}

	const std::vector<std::string>& WorldAnimationHierarchy::GetChildNames() const
	{
		return p_WorldAnimationHierarchy -> p_Info -> m_ChildNames;		
	}
	


	// World

	std::optional<World> World::FromChunk(std::shared_ptr<Container> mainContainer, std::shared_ptr<wrld> worldChunk)
	{
		World out;

		if (worldChunk == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given worldChunk was NULL!");
			return {};
		}

		out.p_World = worldChunk;
		out.m_MainContainer = mainContainer;

		for (const std::shared_ptr<inst> &ic : worldChunk->m_Instances) {
			if (std::optional<Instance> instance = Instance::FromChunk(mainContainer, ic)) {
				out.m_Instances.push_back(*instance);
			}
		}

		std::vector<std::shared_ptr<regn>>& regions = worldChunk -> m_Regions;
		for (size_t i = 0; i < regions.size(); ++i)
		{
			if (std::optional<Region> region = Region::FromChunk(regions[i]))
			{
				out.m_Regions.push_back(*region);
			}
		}

		std::vector<std::shared_ptr<BARR>>& barriers = worldChunk -> m_Barriers;
		for (size_t i = 0; i < barriers.size(); ++i)
		{
			if (std::optional<Barrier> barrier = Barrier::FromChunk(barriers[i]))
			{
				out.m_Barriers.push_back(*barrier);
			}
		}

		std::vector<std::shared_ptr<Hint>>& hintNodes = worldChunk -> m_HintNodes;
		for (size_t i = 0; i < hintNodes.size(); ++i)
		{
			if (std::optional<HintNode> hintNode = HintNode::FromChunk(hintNodes[i]))
			{
				out.m_HintNodes.push_back(*hintNode);
			}
		}

		std::vector<std::shared_ptr<anim>>& animations = worldChunk -> m_Animations;
		for (size_t i = 0; i < animations.size(); ++i)
		{
			if (std::optional<WorldAnimation> anim = WorldAnimation::FromChunk(animations[i]))
			{
				out.m_Animations.push_back(*anim);
			}
		}

		std::vector<std::shared_ptr<anmg>>& animationGroups = worldChunk -> m_AnimationGroups;
		for (size_t i = 0; i < animationGroups.size(); ++i)
		{
			if (std::optional<WorldAnimationGroup> group = WorldAnimationGroup::FromChunk(animationGroups[i]))
			{
				out.m_AnimationGroups.push_back(*group);
			}
		}		

		std::vector<std::shared_ptr<anmh>>& animationHiers = worldChunk -> m_AnimationHierarchies;
		for (size_t i = 0; i < animationHiers.size(); ++i)
		{
			if (std::optional<WorldAnimationHierarchy> hier = WorldAnimationHierarchy::FromChunk(animationHiers[i]))
			{
				out.m_AnimationHierarchies.push_back(*hier);
			}
		}

		return out;
	}

	std::string World::GetName() const
	{
		return p_World->p_Name->m_Text;
	}

	std::vector<Instance> World::GetInstances() const
	{
		return m_Instances;
	}

	const std::vector<Region>& World::GetRegions() const
	{
		return m_Regions;
	}

	const std::vector<Barrier>& World::GetBarriers() const
	{
		return m_Barriers;
	}

	const std::vector<HintNode>& World::GetHintNodes() const
	{
		return m_HintNodes;
	}

	std::string World::GetTerrainName() const
	{
		return p_World->p_TerrainName != nullptr ? p_World->p_TerrainName->m_Text : "";
	}

	const Terrain* World::GetTerrain() const
	{
		if (auto container = m_MainContainer.lock()) {
			if (p_World->p_TerrainName != nullptr) {
				return container->FindTerrain(p_World->p_TerrainName->m_Text);
			}
		}
		return nullptr;
	}
	
	std::string World::GetSkyName() const
	{
		return p_World->p_SkyName != nullptr ? p_World->p_SkyName->m_Text : "";
	}
	
	const std::vector<WorldAnimation>& World::GetAnimations() const
	{
		return m_Animations;
	}

	const std::vector<WorldAnimationGroup>& World::GetAnimationGroups() const
	{
		return m_AnimationGroups;
	}

	const std::vector<WorldAnimationHierarchy>& World::GetAnimationHierarchies() const
	{
		return m_AnimationHierarchies;
	}
}
