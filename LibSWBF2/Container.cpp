#include "pch.h"
#include "Container.h"
#include "InternalHelpers.h"
#include "Hashing.h"
#include "FileReader.h"

#include "Chunks/LVL/LVL.h"

#include <vector>
#include <map>

#include "StreamReader.h"


namespace LibSWBF2
{
	using LibSWBF2::Chunks::GenericBaseChunk;

	// const version
	template<class T1, class T2>
	void CopyMap(std::unordered_map<T2, size_t>& levelMap, const std::vector<T1>& list, std::unordered_map<T2, const T1*>& containerMap)
	{
		for (auto& it : levelMap)
		{
			containerMap.emplace(it.first, &list[it.second]);
		}
	}

	// non const version
	template<class T1, class T2>
	void CopyMap(const std::unordered_map<T2, size_t>& levelMap, std::vector<T1>& list, std::unordered_map<T2, T1*>& containerMap)
	{
		for (auto& it : levelMap)
		{
			containerMap.emplace(it.first, &list[it.second]);
		}
	}

	template<class T>
	void CopyList(const std::vector<T>& srcList, std::vector<const T*>& dstList)
	{
		for (size_t i = 0; i < srcList.size(); ++i)
		{
			dstList.push_back(&srcList[i]);
		}
	}

	std::shared_ptr<Level> Container::LoadLevel(const std::string& path, const std::vector<std::string>* subLVLsToLoad, bool bRegisterContents)
	{
		LIBSWBF2_LOG_INFO("Container::LoadLevel('{}', {}, {})", path, fmt::ptr(subLVLsToLoad), bRegisterContents);

		using LibSWBF2::Chunks::LVL::LVL;

		{
			StreamReader reader;
			if (!reader.Open(path)) {
				return nullptr;
			}
			reader.Close();
		}
		std::shared_ptr<LVL> lvl = std::make_shared<LVL>();

		if (lvl->ReadFile(path, subLVLsToLoad)) {
			std::shared_ptr<Level> level = Level::FromChunk(lvl, shared_from_this());
			if (level) {
				m_Levels.emplace_back(level);
				if (bRegisterContents) {
					CopyMap(level->m_NameToIndexMaps.TextureNameToIndex, level->m_Textures, m_TextureDB);
					CopyMap(level->m_NameToIndexMaps.ModelNameToIndex,level->m_Models, m_ModelDB);
					CopyMap(level->m_NameToIndexMaps.WorldNameToIndex,level->m_Worlds, m_WorldDB);
					CopyMap(level->m_NameToIndexMaps.TerrainNameToIndex,level->m_Terrains, m_TerrainDB);
					CopyMap(level->m_NameToIndexMaps.ScriptNameToIndex,level->m_Scripts, m_ScriptDB);
					CopyMap(level->m_NameToIndexMaps.EntityClassTypeToIndex,level->m_EntityClasses, m_EntityClassDB);
					CopyMap(level->m_NameToIndexMaps.AnimationBankNameToIndex,level->m_AnimationBanks, m_AnimationBankDB);
					CopyMap(level->m_NameToIndexMaps.AnimationSkeletonNameToIndex, level->m_AnimationSkeletons, m_AnimationSkeletonDB);
					CopyMap(level->m_NameToIndexMaps.ConfigHashToIndex, level->m_Configs, m_ConfigDB);
					CopyMap(level->m_NameToIndexMaps.SoundHashToIndex, level->m_Sounds, m_SoundDB);

					CopyList(level->m_Worlds, m_Worlds);


					for (auto& it : level->m_NameToIndexMaps.LocalizationNameToIndex)
					{
						auto find = m_LocalizationDB.find(it.first);
						if (find == m_LocalizationDB.end())
						{
							std::vector<const Localization*> list;
							list.push_back(&level->m_Localizations[it.second]);
							m_LocalizationDB.emplace(it.first, list);
						}
						else
						{
							find->second.push_back(&level->m_Localizations[it.second]);
						}
					}
				}

				level->m_FullPath = path;
				return level;
			}
		}

		return nullptr;
	}

	std::shared_ptr<Level> Container::AddLevel(const std::string &path, const std::vector<std::string>* subLVLsToLoad, bool bRegisterContents)
	{
		LIBSWBF2_LOG_INFO("Container::AddLevel('{}', {}, {})", path, fmt::ptr(subLVLsToLoad), bRegisterContents);
		return LoadLevel(path, subLVLsToLoad, bRegisterContents);
	}

	std::shared_ptr<Level> Container::GetLevel(size_t index) const
	{
		LIBSWBF2_LOG_INFO("Container::GetLevel({})", index);
		if (index >= m_Levels.size())
		{
			LIBSWBF2_LOG_WARN("Given Level index '{}' is illegal!", index);
			return nullptr;
		}

		return m_Levels[index];
	}

	std::shared_ptr<Level> Container::TryGetWorldLevel() const
	{
		LIBSWBF2_LOG_INFO("Container::TryGetWorldLevel()");
		for (size_t i = 0; i < m_Levels.size(); ++i) {
			if (m_Levels[i]->IsWorldLevel()) {
				return m_Levels[i];
			}
		}
		return nullptr;
	}

	const std::vector<const World*>& Container::GetWorlds()
	{
		LIBSWBF2_LOG_INFO("Container::GetWorlds()");
		return m_Worlds;
	}



	// Model
	const Model* Container::FindModel(std::string modelName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindModel('{}')", modelName);
		if (modelName.empty())
		{
			return nullptr;
		}

		return FindModel(FNV::Hash(modelName));
	}

	const Model* Container::FindModel(FNVHash modelName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindModel({})", modelName);
		auto it = m_ModelDB.find(modelName);
		if (it != m_ModelDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Texture
	const Texture* Container::FindTexture(std::string textureName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindTexture('{}')", textureName);
		if (textureName.empty())
		{
			return nullptr;
		}

		return FindTexture(FNV::Hash(textureName));
	}

	const Texture* Container::FindTexture(FNVHash textureName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindTexture({})", textureName);
		auto it = m_TextureDB.find(textureName);
		if (it != m_TextureDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// World
	const World* Container::FindWorld(std::string worldName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindWorld('{}')", worldName);
		if (worldName.empty())
		{
			return nullptr;
		}

		return FindWorld(FNV::Hash(worldName));
	}

	const World* Container::FindWorld(FNVHash worldName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindWorld({})", worldName);
		auto it = m_WorldDB.find(worldName);
		if (it != m_WorldDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Terrain
	const Terrain* Container::FindTerrain(std::string terrainName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindTerrain('{}')", terrainName);
		if (terrainName.empty())
		{
			return nullptr;
		}

		return FindTerrain(FNV::Hash(terrainName));
	}

	const Terrain* Container::FindTerrain(FNVHash terrainName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindTerrain({})", terrainName);
		auto it = m_TerrainDB.find(terrainName);
		if (it != m_TerrainDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Script
	const Script* Container::FindScript(std::string scriptName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindScript('{}')", scriptName);
		if (scriptName.empty())
		{
			return nullptr;
		}

		return FindScript(FNV::Hash(scriptName));
	}

	const Script* Container::FindScript(FNVHash scriptName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindScript({})", scriptName);
		auto it = m_ScriptDB.find(scriptName);
		if (it != m_ScriptDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// AnimationBank
	const AnimationBank* Container::FindAnimationBank(std::string setName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindAnimationBank('{}')", setName);
		if (setName.empty()) return nullptr;

		return FindAnimationBank(FNV::Hash(setName));
	}

	const AnimationBank* Container::FindAnimationBank(FNVHash setName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindAnimationBank({})", setName);
		auto it = m_AnimationBankDB.find(setName);
		if (it != m_AnimationBankDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// AnimationSkeleton
	const AnimationSkeleton* Container::FindAnimationSkeleton(std::string skelName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindAnimationSkeleton('{}')", skelName);
		if (skelName.empty()) return nullptr;

		return FindAnimationSkeleton(FNV::Hash(skelName));
	}

	const AnimationSkeleton* Container::FindAnimationSkeleton(FNVHash skelName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindAnimationSkeleton({})", skelName);
		auto it = m_AnimationSkeletonDB.find(skelName);
		if (it != m_AnimationSkeletonDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Localization
	const std::vector<const Localization*>* Container::FindLocalizations(std::string languageName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindLocalizations('{}')", languageName);
		if (languageName.empty())
		{
			return nullptr;
		}

		return FindLocalizations(FNV::Hash(languageName));
	}

	const std::vector<const Localization*>* Container::FindLocalizations(FNVHash languageName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindLocalizations({})", languageName);
		auto it = m_LocalizationDB.find(languageName);
		if (it != m_LocalizationDB.end())
		{
			return &it->second;
		}

		return nullptr;
	}



	// EntityClass
	const EntityClass* Container::FindEntityClass(std::string typeName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindEntityClass('{}')", typeName);
		if (typeName.empty())
		{
			return nullptr;
		}

		return FindEntityClass(FNV::Hash(typeName));
	}

	const EntityClass* Container::FindEntityClass(FNVHash typeName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindEntityClass({})", typeName);
		auto it = m_EntityClassDB.find(typeName);
		if (it != m_EntityClassDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Sound
	const Sound* Container::FindSound(std::string soundName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindSound('{}')", soundName);
		if (soundName.empty())
		{
			return nullptr;
		}
		return FindSound(FNV::Hash(soundName));
	}

	const Sound* Container::FindSound(FNVHash hashedSoundName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindSound({})", hashedSoundName);
		auto it = m_SoundDB.find(hashedSoundName);
		if (it != m_SoundDB.end())
		{
			return it->second;
		}
		return nullptr;
	}



	// Config
	const Config* Container::FindConfig(EConfigType type, FNVHash hashedConfigName) const
	{
		LIBSWBF2_LOG_INFO("Container::FindConfig({})", hashedConfigName);
		auto it = m_ConfigDB.find(hashedConfigName + (uint32_t) type);
		if (it != m_ConfigDB.end())
		{
			return it->second;
		}
		return nullptr;
	}


	bool Container::GetLocalizedWideString(const std::string& language, const std::string& path, const wchar_t*& chars, uint32_t& count) const
	{
		LIBSWBF2_LOG_INFO("Container::GetLocalizedWideString('{}', '{}', {}, {})", language, path, fmt::ptr(chars), fmt::ptr(&count));
		const std::vector<const Localization*>* locals = FindLocalizations(language);
		if (locals == nullptr)
		{
			LIBSWBF2_LOG_WARN("Cannot find language '{}'!", language);
			return false;
		}
		for (size_t i = 0; i < locals->size(); ++i)
		{
			if ((*locals)[i]->GetLocalizedWideString(path, chars, count))
			{
				return true;
			}
		}
		return false;
	}
}
