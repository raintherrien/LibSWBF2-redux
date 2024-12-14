#pragma once
#include "Wrappers/Level.h"
#include "Wrappers/Terrain.h"
#include "Wrappers/Localization.h"
#include "Wrappers/SoundBank.h"
#include "Wrappers/AnimationSkeleton.h"
#include "Types/Enums.h"
#include <string>
#include <vector>

namespace LibSWBF2
{
	using LibSWBF2::Wrappers::Level;
	using LibSWBF2::Wrappers::SoundBank;
	using LibSWBF2::Wrappers::Sound;
	using LibSWBF2::Wrappers::Model;
	using LibSWBF2::Wrappers::Texture;
	using LibSWBF2::Wrappers::World;
	using LibSWBF2::Wrappers::Terrain;
	using LibSWBF2::Wrappers::Script;
	using LibSWBF2::Wrappers::Localization;
	using LibSWBF2::Wrappers::EntityClass;
	using LibSWBF2::Wrappers::AnimationBank;
	using LibSWBF2::Wrappers::AnimationSkeleton;
	using LibSWBF2::Wrappers::Config;

	// Procedure:
	// - Load levels and sound banks via AddLevel
	// - Repeat as many times as you want
	class LIBSWBF2_API Container
	{
	private:
		Container() = default;
		~Container() = default;

		std::vector<Level *> m_Levels;

		// TODO: rework once we allow modifications (add / delete) in Levels
		std::unordered_map<FNVHash, const Texture*> m_TextureDB;
		std::unordered_map<FNVHash,	const Model*> m_ModelDB;
		std::unordered_map<FNVHash, const World*> m_WorldDB;
		std::unordered_map<FNVHash, const Terrain*> m_TerrainDB;
		std::unordered_map<FNVHash, const Script*> m_ScriptDB;
		std::unordered_map<FNVHash, const EntityClass*> m_EntityClassDB;
		std::unordered_map<FNVHash, const AnimationBank*> m_AnimationBankDB;
		std::unordered_map<FNVHash, const AnimationSkeleton*> m_AnimationSkeletonDB;
		std::unordered_map<FNVHash, const Config*> m_ConfigDB;
		std::unordered_map<FNVHash, const Sound*> m_SoundDB;
		std::unordered_map<FNVHash, std::vector<const Localization*>> m_LocalizationDB;


		std::vector<const World*> m_Worlds;

		uint64_t m_OverallSize = 0;

		Level *LoadLevel(const std::string& path, const std::vector<std::string>* subLVLsToLoad, bool bRegisterContents);

	public:
		static Container* Create();
		static void Delete(Container* instance);

		Level *AddLevel(const std::string &path, const std::vector<std::string>* subLVLsToLoad = nullptr, bool bRegisterContents=true);
		Level *GetLevel(size_t index) const;

		// will return the first encountered world LVL, if existent
		Level* TryGetWorldLevel() const;

		const std::vector<const World*>& GetWorlds();

		const Model* FindModel(std::string modelName) const;
		const Texture* FindTexture(std::string textureName) const;
		const World* FindWorld(std::string worldName) const;
		const Terrain* FindTerrain(std::string terrainName) const;
		const Script* FindScript(std::string scriptName) const;
		const EntityClass* FindEntityClass(std::string typeName) const;
		const Sound* FindSound(std::string soundName) const;
		const AnimationBank* FindAnimationBank(std::string setName) const;
		const AnimationSkeleton* FindAnimationSkeleton(std::string skelName) const;


		const Model* FindModel(FNVHash modelName) const;
		const Texture* FindTexture(FNVHash textureName) const;
		const World* FindWorld(FNVHash worldName) const;
		const Terrain* FindTerrain(FNVHash terrainName) const;
		const Script* FindScript(FNVHash scriptName) const;
		const EntityClass* FindEntityClass(FNVHash typeName) const;
		const Sound* FindSound(FNVHash hashedSoundName) const;
		const AnimationBank* FindAnimationBank(FNVHash setName) const;
		const AnimationSkeleton* FindAnimationSkeleton(FNVHash skelName) const;


		const Config* FindConfig(EConfigType type, FNVHash hashedConfigName) const;

		const std::vector<const Localization*>* FindLocalizations(std::string languageName) const;
		const std::vector<const Localization*>* FindLocalizations(FNVHash languageName) const;

		// count is number of wide chars, NOT number of bytes!
		bool GetLocalizedWideString(const std::string& language, const std::string& path, const wchar_t*& chars, uint32_t& count) const;
	};
}
