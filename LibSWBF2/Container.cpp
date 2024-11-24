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
	void CopyMap(std::unordered_map<T2, size_t>& levelMap, const List<T1>& list, std::unordered_map<T2, const T1*>& containerMap)
	{
		for (auto& it : levelMap)
		{
			containerMap.emplace(it.first, &list[it.second]);
		}
	}

	// non const version
	template<class T1, class T2>
	void CopyMap(std::unordered_map<T2, size_t>& levelMap, List<T1>& list, std::unordered_map<T2, T1*>& containerMap)
	{
		for (auto& it : levelMap)
		{
			containerMap.emplace(it.first, &list[it.second]);
		}
	}

	template<class T>
	void CopyList(const List<T>& srcList, List<const T*>& dstList)
	{
		for (size_t i = 0; i < srcList.Size(); ++i)
		{
			dstList.Add(&srcList[i]);
		}
	}

	struct LoadStatus // XXX No longer used
	{
		Level* m_Level = nullptr;
	};

	Level *Container::LoadLevel(const String& path, const List<String>* subLVLsToLoad, bool bRegisterContents)
	{
		using LibSWBF2::Chunks::LVL::LVL;

		LVL* lvl = nullptr;
		GenericBaseChunk* chunk = nullptr;
		{
			StreamReader reader;
			if (!reader.Open(path)) {
				return nullptr;
			}
			reader.Close();

			lvl = LVL::Create();
			chunk = lvl;
		}

		if (lvl->ReadFromFile(path, subLVLsToLoad)) {
			Level* level = Level::FromChunk(lvl, this);
			if (level != nullptr) {
				if (bRegisterContents) {
					CopyMap(level->m_NameToIndexMaps->TextureNameToIndex,			level->m_Textures,			 m_TextureDB);
					CopyMap(level->m_NameToIndexMaps->ModelNameToIndex,				level->m_Models,			 m_ModelDB);
					CopyMap(level->m_NameToIndexMaps->WorldNameToIndex,				level->m_Worlds,			 m_WorldDB);
					CopyMap(level->m_NameToIndexMaps->TerrainNameToIndex,			level->m_Terrains,			 m_TerrainDB);
					CopyMap(level->m_NameToIndexMaps->ScriptNameToIndex,			level->m_Scripts,			 m_ScriptDB);
					CopyMap(level->m_NameToIndexMaps->EntityClassTypeToIndex,		level->m_EntityClasses, 	 m_EntityClassDB);
					CopyMap(level->m_NameToIndexMaps->AnimationBankNameToIndex,		level->m_AnimationBanks,     m_AnimationBankDB);
					CopyMap(level->m_NameToIndexMaps->AnimationSkeletonNameToIndex,	level->m_AnimationSkeletons, m_AnimationSkeletonDB);
					CopyMap(level->m_NameToIndexMaps->ConfigHashToIndex,			level->m_Configs, 			 m_ConfigDB);
					CopyMap(level->m_NameToIndexMaps->SoundHashToIndex,         	level->m_Sounds,        	 m_SoundDB);

					CopyList(level->m_Worlds, m_Worlds);


					for (auto& it : level->m_NameToIndexMaps->LocalizationNameToIndex)
					{
						auto find = m_LocalizationDB.find(it.first);
						if (find == m_LocalizationDB.end())
						{
							List<const Localization*> list;
							list.Add(&level->m_Localizations[it.second]);
							m_LocalizationDB.emplace(it.first, list);
						}
						else
						{
							find->second.Add(&level->m_Localizations[it.second]);
						}
					}
				}

				level->m_FullPath = path;
				return level;
			} else {
				LVL::Destroy(lvl);
				return nullptr;
			}
		} else {
			LVL::Destroy(lvl);
			return nullptr;
		}
	}

	Container* Container::Create()
	{
		return new Container();
	}

	void Container::Delete(Container* instance)
	{
		delete instance;
	}

	Level *Container::AddLevel(const String& path, const List<String>* subLVLsToLoad, bool bRegisterContents)
	{
		Level *lvl = LoadLevel(path, subLVLsToLoad, bRegisterContents);
		if (lvl) {
			m_Levels.Add(lvl);
		}
		return lvl;
	}

	Level* Container::GetLevel(size_t index) const
	{
		if (index >= m_Levels.Size())
		{
			LOG_WARN("Given Level index '{}' is illegal!", index);
			return nullptr;
		}

		return m_Levels[index];
	}

	Level* Container::TryGetWorldLevel() const
	{
		for (size_t i = 0; i < m_Levels.Size(); ++i) {
			if (m_Levels[i]->IsWorldLevel()) {
				return m_Levels[i];
			}
		}
		return nullptr;
	}

	const List<const World*>& Container::GetWorlds()
	{
		return m_Worlds;
	}



	// Model
	const Model* Container::FindModel(String modelName) const
	{
		if (modelName.IsEmpty())
		{
			return nullptr;
		}

		return FindModel(FNV::Hash(modelName));
	}

	const Model* Container::FindModel(FNVHash modelName) const
	{
		auto it = m_ModelDB.find(modelName);
		if (it != m_ModelDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Texture
	const Texture* Container::FindTexture(String textureName) const
	{
		if (textureName.IsEmpty())
		{
			return nullptr;
		}

		return FindTexture(FNV::Hash(textureName));
	}

	const Texture* Container::FindTexture(FNVHash textureName) const
	{
		auto it = m_TextureDB.find(textureName);
		if (it != m_TextureDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// World
	const World* Container::FindWorld(String worldName) const
	{
		if (worldName.IsEmpty())
		{
			return nullptr;
		}

		return FindWorld(FNV::Hash(worldName));
	}

	const World* Container::FindWorld(FNVHash worldName) const
	{
		auto it = m_WorldDB.find(worldName);
		if (it != m_WorldDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Terrain
	const Terrain* Container::FindTerrain(String terrainName) const
	{
		if (terrainName.IsEmpty())
		{
			return nullptr;
		}

		return FindTerrain(FNV::Hash(terrainName));
	}

	const Terrain* Container::FindTerrain(FNVHash terrainName) const
	{
		auto it = m_TerrainDB.find(terrainName);
		if (it != m_TerrainDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Script
	const Script* Container::FindScript(String scriptName) const
	{
		if (scriptName.IsEmpty())
		{
			return nullptr;
		}

		return FindScript(FNV::Hash(scriptName));
	}

	const Script* Container::FindScript(FNVHash scriptName) const
	{
		auto it = m_ScriptDB.find(scriptName);
		if (it != m_ScriptDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// AnimationBank
	const AnimationBank* Container::FindAnimationBank(String setName) const
	{
		if (setName.IsEmpty()) return nullptr;

		return FindAnimationBank(FNV::Hash(setName));
	}

	const AnimationBank* Container::FindAnimationBank(FNVHash setName) const
	{
		auto it = m_AnimationBankDB.find(setName);
		if (it != m_AnimationBankDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// AnimationSkeleton
	const AnimationSkeleton* Container::FindAnimationSkeleton(String skelName) const
	{
		if (skelName.IsEmpty()) return nullptr;

		return FindAnimationSkeleton(FNV::Hash(skelName));
	}

	const AnimationSkeleton* Container::FindAnimationSkeleton(FNVHash skelName) const
	{
		auto it = m_AnimationSkeletonDB.find(skelName);
		if (it != m_AnimationSkeletonDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Localization
	const List<const Localization*>* Container::FindLocalizations(String languageName) const
	{
		if (languageName.IsEmpty())
		{
			return nullptr;
		}

		return FindLocalizations(FNV::Hash(languageName));
	}

	const List<const Localization*>* Container::FindLocalizations(FNVHash languageName) const
	{
		auto it = m_LocalizationDB.find(languageName);
		if (it != m_LocalizationDB.end())
		{
			return &it->second;
		}

		return nullptr;
	}



	// EntityClass
	const EntityClass* Container::FindEntityClass(String typeName) const
	{
		if (typeName.IsEmpty())
		{
			return nullptr;
		}

		return FindEntityClass(FNV::Hash(typeName));
	}

	const EntityClass* Container::FindEntityClass(FNVHash typeName) const
	{
		auto it = m_EntityClassDB.find(typeName);
		if (it != m_EntityClassDB.end())
		{
			return it->second;
		}

		return nullptr;
	}



	// Sound
	const Sound* Container::FindSound(String soundName) const
	{
		if (soundName.IsEmpty())
		{
			return nullptr;
		}
		return FindSound(FNV::Hash(soundName));
	}

	const Sound* Container::FindSound(FNVHash hashedSoundName) const
	{
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
		auto it = m_ConfigDB.find(hashedConfigName + (uint32_t) type);
		if (it != m_ConfigDB.end())
		{
			return it->second;
		}
		return nullptr;
	}


	bool Container::GetLocalizedWideString(const String& language, const String& path, uint16_t*& chars, uint32_t& count) const
	{
		const List<const Localization*>* locals = FindLocalizations(language);
		if (locals == nullptr)
		{
			LOG_WARN("Cannot find language '{}'!", language);
			return false;
		}
		for (size_t i = 0; i < locals->Size(); ++i)
		{
			if ((*locals)[i]->GetLocalizedWideString(path, chars, count))
			{
				return true;
			}
		}
		return false;
	}
}
