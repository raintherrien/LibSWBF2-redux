#include "stdafx.h"
#include "Level.h"
#include "InternalHelpers.h"
#include "Chunks/LVL/tex_/tex_.h"
#include "Chunks/LVL/modl/LVL.modl.h"
#include "Chunks/LVL/scr_/scr_.h"
#include <unordered_map>

namespace LibSWBF2::Wrappers
{
	using Chunks::GenericBaseChunk;
	using Chunks::LVL::texture::tex_;
	using Chunks::LVL::modl::modl;
	using Chunks::LVL::terrain::tern;

	// TODO: better solution than having a global hash map for all levels. cannot put in header aswell though...
	std::unordered_map<std::string, size_t> TextureNameToIndex;
	std::unordered_map<std::string, size_t> ModelNameToIndex;
	std::unordered_map<std::string, size_t> WorldNameToIndex;
	std::unordered_map<std::string, size_t> TerrainNameToIndex;
	std::unordered_map<std::string, size_t> ScriptNameToIndex;

	std::string ToLower(String name)
	{
		std::string result = name.Buffer();
		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return result;
	}

	Level::Level(LVL* lvl)
	{
		p_lvl = lvl;
	}

	Level::~Level()
	{
		m_Models.Clear();
		m_Textures.Clear();

		if (p_lvl == nullptr)
		{
			LOG_ERROR("p_lvl of Level was NULL!");
		}
		else
		{
			LVL::Destroy(p_lvl);
		}
	}

	void Level::FindInChildrenRecursive(GenericBaseChunk* root)
	{
		// IMPORTANT: crawl textures BEFORE models, so texture references via string can be resolved in models
		tex_* textureChunk = dynamic_cast<tex_*>(root);
		if (textureChunk != nullptr)
		{
			Texture texture;
			if (Texture::FromChunk(textureChunk, texture))
			{
				TextureNameToIndex.emplace(ToLower(texture.GetName()), m_Textures.Add(texture));
			}
		}

		// IMPORTANT: crawl models BEFORE worlds, so model references via string can be resolved in worlds
		modl* modelChunk = dynamic_cast<modl*>(root);
		if (modelChunk != nullptr)
		{
			Model model;
			if (Model::FromChunk(this, modelChunk, model))
			{
				ModelNameToIndex.emplace(ToLower(model.GetName()), m_Models.Add(model));
			}
		}

		wrld* worldChunk = dynamic_cast<wrld*>(root);
		if (worldChunk != nullptr)
		{
			World world;
			if (World::FromChunk(this, worldChunk, world))
			{
				WorldNameToIndex.emplace(ToLower(world.GetName()), m_Worlds.Add(world));
			}
		}

		tern* terrainChunk = dynamic_cast<tern*>(root);
		if (terrainChunk != nullptr)
		{
			Terrain terrain;
			if (Terrain::FromChunk(this, terrainChunk, terrain))
			{
				TerrainNameToIndex.emplace(ToLower(terrain.GetName()), m_Terrains.Add(terrain));
			}
		}

		scr_* scriptChunk = dynamic_cast<scr_*>(root);
		if (scriptChunk != nullptr)
		{
			Script script;
			if (Script::FromChunk(scriptChunk, script))
			{
				ScriptNameToIndex.emplace(ToLower(script.GetName()), m_Scripts.Add(script));
			}
		}

		const List<GenericBaseChunk*>& children = root->GetChildren();
		for (size_t i = 0; i < children.Size(); ++i)
		{
			FindInChildrenRecursive(children[i]);
		}
	}

	Level* Level::FromFile(String path)
	{
		LVL* lvl = LVL::Create();
		if (!lvl->ReadFromFile(path.Buffer()))
		{
			LVL::Destroy(lvl);
			return nullptr;
		}
		
		Level* result = new Level(lvl);
		result->FindInChildrenRecursive(lvl);

		return result;
	}

	void Level::Destroy(Level* level)
	{
		if (level == nullptr)
		{
			LOG_ERROR("Given level was NULL!");
			return;
		}

		delete level;
	}

	const List<Model>& Level::GetModels() const
	{
		return m_Models;
	}

	const List<Texture>& Level::GetTextures() const
	{
		return m_Textures;
	}

	const List<World>& Level::GetWorlds() const
	{
		return m_Worlds;
	}

	const List<Terrain>& Level::GetTerrains() const
	{
		return m_Terrains;
	}

	const List<Script>& Level::GetScripts() const
	{
		return m_Scripts;
	}

	const Model* Level::GetModel(String modelName) const
	{
		if (modelName == "")
		{
			return nullptr;
		}

		auto it = ModelNameToIndex.find(ToLower(modelName));
		if (it != ModelNameToIndex.end())
		{
			return &m_Models[it->second];
		}

		//LOG_WARN("Could not find Model '{}'!", modelName);
		return nullptr;
	}

	const Texture* Level::GetTexture(String textureName) const
	{
		if (textureName == "")
		{
			return nullptr;
		}

		auto it = TextureNameToIndex.find(ToLower(textureName));
		if (it != TextureNameToIndex.end())
		{
			return &m_Textures[it->second];
		}

		//LOG_WARN("Could not find Texture '{}'!", textureName);
		return nullptr;
	}

	const World* Level::GetWorld(String worldName) const
	{
		if (worldName == "")
		{
			return nullptr;
		}

		auto it = WorldNameToIndex.find(ToLower(worldName));
		if (it != WorldNameToIndex.end())
		{
			return &m_Worlds[it->second];
		}

		//LOG_WARN("Could not find World '{}'!", worldName);
		return nullptr;
	}

	const Terrain* Level::GetTerrain(String terrainName) const
	{
		if (terrainName == "")
		{
			return nullptr;
		}

		auto it = TerrainNameToIndex.find(ToLower(terrainName));
		if (it != TerrainNameToIndex.end())
		{
			return &m_Terrains[it->second];
		}

		//LOG_WARN("Could not find Terrain '{}'!", terrainName);
		return nullptr;
	}

	const Script* Level::GetScript(String scriptName) const
	{
		if (scriptName == "")
		{
			return nullptr;
		}

		auto it = ScriptNameToIndex.find(ToLower(scriptName));
		if (it != ScriptNameToIndex.end())
		{
			return &m_Scripts[it->second];
		}

		//LOG_WARN("Could not find Script '{}'!", scriptName);
		return nullptr;
	}
}