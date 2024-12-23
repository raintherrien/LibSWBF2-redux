#pragma once
#include "req.h"
#include "AnimationBank.h"
#include "AnimationSkeleton.h"
#include "Config.h"
#include "EntityClass.h"
#include "Localization.h"
#include "Model.h"
#include "PlanSet.h"
#include "Script.h"
#include "Sound.h"
#include "SoundStream.h"
#include "Texture.h"
#include "World.h"

#include <string>
#include <vector>

namespace LibSWBF2
{
	namespace Chunks {
		struct GenericBaseChunk;

		namespace LVL
		{
			struct LVL;
			namespace skel 
			{
				struct skel;
			}
			namespace sound
			{
				struct Stream;
			}
		}
	}

	class Container;
}


namespace LibSWBF2::Wrappers
{
	/*
	 * This and the other wrapper classes just serve as abstraction Layers
	 * to wrap around the chunk data. You can of course just use the chunk data
	 * directly if you want. This is just to make access to desired data more
	 * straight forward.
	 */
	class LIBSWBF2_API Level : public std::enable_shared_from_this<Level>
	{

	typedef LibSWBF2::Chunks::LVL::LVL LVL;
	typedef LibSWBF2::Chunks::GenericBaseChunk GenericBaseChunk;
	typedef LibSWBF2::Chunks::LVL::skel::skel skel;
	typedef LibSWBF2::Chunks::LVL::sound::Stream Stream;


	private:
		friend Container;

		std::shared_ptr<LVL> p_lvl;
		std::weak_ptr<Container> p_MainContainer;
		std::string m_FullPath;

		std::vector<Model> m_Models;
		std::vector<Texture> m_Textures;
		std::vector<World> m_Worlds;
		std::vector<Terrain> m_Terrains;	// multiple terrains are possible, in theory. never saw it though
		std::vector<Script> m_Scripts;
		std::vector<Localization> m_Localizations;
		std::vector<EntityClass> m_EntityClasses;
		std::vector<AnimationBank> m_AnimationBanks;
		std::vector<AnimationSkeleton> m_AnimationSkeletons;
		std::vector<Sound> m_Sounds;
		std::vector<Config> m_Configs;
		std::vector<SoundStream> m_SoundStreams;
		std::vector<SoundBank> m_SoundBanks;

		std::vector<PlanSet> m_PlanSets;

		class MapsWrapper m_NameToIndexMaps;

		SoundStream * WrapStreamChunk(std::shared_ptr<Stream> streamChunk);

	public:
		friend class Model;
		friend class Segment;

		Level() = default;
		Level(std::shared_ptr<LVL> lvl, std::shared_ptr<Container> mainContainer);

		// subLVLsToLoad doesn't need to be persistent, can be a stack value.
		// contents will be copied and hashed.
		static std::shared_ptr<Level> FromFile(const std::string& path, const std::vector<std::string>* subLVLsToLoad = nullptr);
		static std::shared_ptr<Level> FromChunk(std::shared_ptr<LVL> lvl, std::shared_ptr<Container> mainContainer);
		static std::shared_ptr<Level> FromStream(FileReader& Stream);

		const std::string& GetLevelPath() const;
		std::string GetLevelName() const;
		bool IsWorldLevel() const;

		const std::vector<Model>& GetModels() const;
		const std::vector<Texture>& GetTextures() const;
		const std::vector<World>& GetWorlds() const;
		const std::vector<Terrain>& GetTerrains() const;
		const std::vector<Script>& GetScripts() const;
		const std::vector<Localization>& GetLocalizations() const;
		const std::vector<EntityClass>& GetEntityClasses() const;
		const std::vector<AnimationBank>& GetAnimationBanks() const;
		const std::vector<AnimationSkeleton>& GetAnimationSkeletons() const;
		const std::vector<Sound>& GetSounds() const;
		const std::vector<const Config *> GetConfigs(EConfigType cfgType = EConfigType::All) const;
		const std::vector<SoundStream>& GetSoundStreams() const;
		const std::vector<SoundBank>& GetSoundBanks() const;
		const std::vector<PlanSet>& GetPlanSets() const;

		const Model* GetModel(const std::string& modelName) const;
		const Texture* GetTexture(const std::string& textureName) const;
		const World* GetWorld(const std::string& worldName) const;
		const Terrain* GetTerrain(const std::string& terrainName) const;
		const Script* GetScript(const std::string& scriptName) const;
		const Localization* GetLocalization(const std::string& loclName) const;
		const EntityClass* GetEntityClass(const std::string& typeName) const;
		const AnimationBank* GetAnimationBank(const std::string& setName) const;
		const AnimationSkeleton* GetAnimationSkeleton(const std::string& skelName) const;
		const Sound* GetSound(const std::string& soundHashName) const;
		const Config* GetConfig(EConfigType cfgType, const std::string& cfgName) const;
		const SoundStream* GetSoundStream(const std::string& streamName) const;
		const SoundBank* GetSoundBank(const std::string& bankName) const;

		const Model* GetModel(FNVHash modelName) const;
		const Texture* GetTexture(FNVHash textureName) const;
		const World* GetWorld(FNVHash worldName) const;
		const Terrain* GetTerrain(FNVHash terrainName) const;
		const Script* GetScript(FNVHash scriptName) const;
		const Localization* GetLocalization(FNVHash loclName) const;
		const EntityClass* GetEntityClass(FNVHash typeName) const;
		const AnimationBank* GetAnimationBank(FNVHash setName) const;
		const AnimationSkeleton* GetAnimationSkeleton(FNVHash skelName) const;
		const Sound* GetSound(FNVHash soundHashName) const;
		const Config* GetConfig(EConfigType cfgType, FNVHash hash) const;
		const SoundStream* GetSoundStream(FNVHash streamHashName) const;
		const SoundBank* GetSoundBank(FNVHash bankHashName) const;


		const LVL* GetChunk() const;

		SoundStream* FindAndIndexSoundStream(FileReader& stream, FNVHash StreamName);


	private:
		void ExploreChildrenRecursive(std::shared_ptr<GenericBaseChunk> root);
		std::shared_ptr<skel> FindSkeleton(const std::string& skeletonName) const;
		std::shared_ptr<skel> FindSkeleton(FNVHash skeletonName) const;
	};
}
