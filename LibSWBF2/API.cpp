#include "pch.h"
#include "API.h"
#include "InternalHelpers.h"
#include "Types/Enums.h"
#include "Types/Planning.h"
#include "Chunks/MSH/MSH.h"

#include <string.h>
#include "Container.h"
#include "Wrappers/Wrappers.h"

#include "Chunks/HeaderNames.h"

#include "MemoryMappedReader.h"
#include "StreamReader.h"

#include <string>
#include <cstring>

namespace LibSWBF2
{
#define CHECKNULL(PTR, ...) if (PTR == nullptr) { LOG_ERROR("Given Pointer was NULL!"); return __VA_ARGS__; }

	template<typename T> auto catchall(T &&t) noexcept -> decltype(t())
	{
		try {
			return t();
		} catch (const std::exception &e) {
			LOG_ERROR("%s", e.what());
		} catch (...) {
			LOG_ERROR("Unknown exception");
		}

		return decltype(t()){};
	}

	static void copy_c_str(const std::string &s, char *c_str, size_t maxlen) noexcept
	{
		if (c_str) {
			std::strncpy(c_str, s.c_str(), maxlen);
			c_str[maxlen - 1] = '\0';
		}
	}

	template<typename T> TList<T> vector_to_tlist(std::vector<T> &&v)
	{
		static_assert(sizeof(TList<T>) == sizeof(CList));
		static_assert(std::is_standard_layout_v<TList<T>>);

		TList<T> l{nullptr, v.size(), sizeof(T)};
		if (l.size > 0) {
			l.elements = malloc(l.size * sizeof(T));
			if constexpr (std::is_trivially_copyable_v<T>) {
				std::memcpy(l.elements, v.data(), l.element_size * l.size);
			} else {
				for (size_t i = 0; i < l.size; ++ i) {
					new(reinterpret_cast<T *>(reinterpret_cast<char *>(l.elements) + i * sizeof(T))) T(v[i]);
				}
			}
		}
		return l;
	}

	template<typename T> TList<T> vector_to_tlist(const std::vector<T> &v)
	{
		static_assert(sizeof(TList<T>) == sizeof(CList));
		static_assert(std::is_standard_layout_v<TList<T>>);

		TList<T> l{nullptr, v.size(), sizeof(T)};
		if (l.size > 0) {
			l.elements = malloc(l.size * sizeof(T));
			if constexpr (std::is_trivially_copyable_v<T>) {
				std::memcpy(l.elements, v.data(), l.element_size * l.size);
			} else {
				for (size_t i = 0; i < l.size; ++ i) {
					new(reinterpret_cast<T *>(reinterpret_cast<char *>(l.elements) + i * sizeof(T))) T(v[i]);
				}
			}
		}
		return l;
	}

	// API List //

	void CList_free(struct CList *l) noexcept
	{
		free(l->elements);
	}

	// Bone //

	Vector3 Bone_GetPosition(const Bone *bone) noexcept
	{
		return catchall([&] {
			CHECKNULL(bone, Vector3{});
			return bone->m_Position;
		});
	}

	Vector4 Bone_GetRotation(const Bone *bone) noexcept
	{
		return catchall([&] {
			CHECKNULL(bone, Vector4{});
			return bone->m_Rotation;
		});
	}

	size_t Bone_GetName(const Bone *bone, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(bone, static_cast<size_t>(0));
			std::string name = bone->m_BoneName;
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	size_t Bone_GetParentName(const Bone *bone, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(bone, static_cast<size_t>(0));
			std::string parent_name = bone->m_Parent;
			copy_c_str(parent_name, out_buffer, out_buffer_len);
			return parent_name.size();
		});
	}

	// Collision Mesh //

	TList<uint16_t> CollisionMesh_GetIndexBuffer(const CollisionMesh *mesh) noexcept
	{
		return catchall([&] {
			CHECKNULL(mesh, TList<uint16_t>{});
			return vector_to_tlist(mesh->GetIndexBuffer());
		});
	}

	TList<Vector3> CollisionMesh_GetVertexBuffer(const CollisionMesh *mesh) noexcept
	{
		return catchall([&] {
			CHECKNULL(mesh, TList<Vector3>{});
			return vector_to_tlist(mesh->GetVertexBuffer());
		});
	}

	// CollisionPrimitive //

	Vector3 CollisionPrimitive_GetPosition(const CollisionPrimitive *cp) noexcept
	{
		return catchall([&] {
			CHECKNULL(cp, Vector3{});
			return cp->GetPosition();
		});
	}

	Vector4 CollisionPrimitive_GetRotation(const CollisionPrimitive *cp) noexcept
	{
		return catchall([&] {
			CHECKNULL(cp, Vector4{});
			return cp->GetRotation();
		});
	}

	size_t CollisionPrimitive_GetParentName(const CollisionPrimitive *cp, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(cp, static_cast<size_t>(0));
			std::string parent_name = cp->GetParentName();
			copy_c_str(parent_name, out_buffer, out_buffer_len);
			return parent_name.size();
		});
	}

	ECollisionPrimitiveType CollisionPrimitive_GetType(const CollisionPrimitive *cp) noexcept
	{
		return catchall([&] {
			CHECKNULL(cp, static_cast<ECollisionPrimitiveType>(0));
			return cp->GetPrimitiveType();
		});
	}

	void CollisionPrimitive_GetCubeDims(const CollisionPrimitive *cp, float *sx, float *sy, float *sz) noexcept
	{
		catchall([&] {
			if (cp == nullptr) {
				*sx = 0.0f;
				*sy = 0.0f;
				*sz = 0.0f;
			} else {
				cp->GetCubeDims(*sx, *sy, *sz);
			}
		});
	}

	void CollisionPrimitive_GetCylinderDims(const CollisionPrimitive *cp, float *sr, float *sh) noexcept
	{
		catchall([&] {
			if (cp == nullptr) {
				*sr = 0.0f;
				*sh = 0.0f;
			} else {
				cp->GetCylinderDims(*sr, *sh);
			}
		});
	}

	void CollisionPrimitive_GetSphereRadius(const CollisionPrimitive *cp, float *sr) noexcept
	{
		catchall([&] {
			if (cp == nullptr) {
				*sr = 0.0f;
			} else {
				cp->GetSphereRadius(*sr);
			}
		});
	}

	// Config //

	const Field *Config_GetField(const Config *cfg, uint32_t name_hash) noexcept
	{
		return catchall([&] {
			CHECKNULL(cfg, static_cast<const Field *>(nullptr));
			return cfg->GetField(name_hash);
		});
	}

	TList<const Field *> Config_GetFields(const Config *cfg, uint32_t name_hash) noexcept
	{
		return catchall([&] {
			CHECKNULL(cfg, TList<const Field *>{});
			return vector_to_tlist(cfg->GetFields(name_hash));
		});
	}


	// Container //

	Container *Container_Create() noexcept
	{
		return catchall([&] {
			return Container::Create();
		});
	}

	void Container_Destroy(Container *container) noexcept
	{
		catchall([&] {
			if (container) {
				Container::Delete(container);
			}
		});
	}

	const Level *Container_AddLevel(Container *container, const char *lvl_path) noexcept
	{
		return catchall([&] {
			CHECKNULL(container, static_cast<const Level *>(nullptr))
			return const_cast<const Level *>(container->AddLevel(lvl_path));
		});
	}

	const Config *Container_FindConfig(Container *container, EConfigType type, uint32_t name_hash) noexcept
	{
		return catchall([&] {
			CHECKNULL(container, static_cast<const Config *>(nullptr));
			return container->FindConfig(type, name_hash);
		});
	}

	const Texture *Container_FindTexture(Container *container, uint32_t name_hash) noexcept
	{
		return catchall([&] {
			CHECKNULL(container, static_cast<const Texture *>(nullptr));
			return container->FindTexture(name_hash);
		});
	}

	const Model *Container_FindModel(Container *container, uint32_t name_hash) noexcept
	{
		return catchall([&] {
			CHECKNULL(container, static_cast<const Model *>(nullptr));
			return container->FindModel(name_hash);
		});
	}

	const EntityClass *Container_FindEntityClass(Container *container, uint32_t name_hash) noexcept
	{
		return catchall([&] {
			CHECKNULL(container, static_cast<const EntityClass *>(nullptr));
			return container->FindEntityClass(name_hash);
		});
	}

	// EntityClass //

	size_t EntityClass_GetBaseName(const EntityClass *ec, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(ec, static_cast<size_t>(0));
			std::string base_name = ec->GetBaseName();
			copy_c_str(base_name, out_buffer, out_buffer_len);
			return base_name.size();
		});
	}
	TList<uint32_t> EntityClass_GetAllPropertyHashes(const struct EntityClass *ec) noexcept
	{
		return catchall([&] {
			CHECKNULL(ec, TList<uint32_t>{});
			return vector_to_tlist(ec->GetAllPropertyHashes());
		});
	}

	size_t EntityClass_GetPropertyValue(const struct EntityClass *ec, uint32_t name_hash, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(ec, static_cast<size_t>(0));
			std::string value;
			if (!ec->GetProperty(name_hash, value)) {
				return static_cast<size_t>(0);
			}
			copy_c_str(value, out_buffer, out_buffer_len);
			return value.size();
		});
	}

	// Field //

	const Scope *Field_GetScope(const Field *field) noexcept
	{
		return catchall([&] {
			CHECKNULL(field, static_cast<const Scope *>(nullptr));
			return &field->m_Scope;
		});
	}

	size_t Field_GetString(const Field *cfg, size_t index, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(cfg, static_cast<size_t>(0));
			std::string s = cfg->GetString(index);
			copy_c_str(s, out_buffer, out_buffer_len);
			return s.size();
		});
	}

	// Helpers //

	uint32_t FNVHashString(const char *string) noexcept
	{
		return catchall([&] {
			return FNV::Hash(string);
		});
	}

	// Instance //

	size_t Instance_GetName(const Instance *instance, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(instance, static_cast<size_t>(0));
			std::string name = instance->GetName();
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	size_t Instance_GetEntityClassName(const Instance *instance, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(instance, static_cast<size_t>(0));
			std::string name = instance->GetEntityClassName();
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	Vector3 Instance_GetPosition(const Instance *instance) noexcept
	{
		return catchall([&] {
			CHECKNULL(instance, Vector3{});
			return instance->GetPosition();
		});
	}

	Vector4 Instance_GetRotation(const Instance *instance) noexcept
	{
		return catchall([&] {
			CHECKNULL(instance, Vector4{});
			return instance->GetRotation();
		});
	}

	// Level //

	TList<World> Level_GetWorlds(const Level *level) noexcept
	{
		return catchall([&] {
			CHECKNULL(level, TList<World>{});
			return vector_to_tlist(level->GetWorlds());
		});
	}

	bool Level_IsWorldLevel(const Level *level) noexcept
	{
		return catchall([&] {
			CHECKNULL(level, false);
			return level->IsWorldLevel();
		});
	}

	size_t Level_GetName(const Level *level, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(level, static_cast<size_t>(0));
			std::string level_name = level->GetLevelName();
			copy_c_str(level_name, out_buffer, out_buffer_len);
			return level_name.size();
		});
	}

	// Material //

	const Texture *Material_GetTexture(const Material *mat, size_t index) noexcept
	{
		return catchall([&] {
			CHECKNULL(mat, static_cast<const Texture *>(nullptr));
			return mat->GetTexture(index);
		});
	}

	EMaterialFlags Material_GetFlags(const Material *mat) noexcept
	{
		return catchall([&] {
			CHECKNULL(mat, static_cast<EMaterialFlags>(0));
			return mat->GetFlags();
		});
	}

	// Model //

	TList<Segment> Model_GetSegments(const Model *model) noexcept
	{
		return catchall([&] {
			CHECKNULL(model, TList<Segment>{});
			return vector_to_tlist(model->GetSegments());
		});
	}

	TList<Bone> Model_GetBones(const Model *model) noexcept
	{
		return catchall([&] {
			CHECKNULL(model, TList<Bone>{});
			return vector_to_tlist(model->GetBones());
		});
	}

	TList<CollisionPrimitive> Model_GetCollisionPrimitives(const Model *model) noexcept
	{
		return catchall([&] {
			CHECKNULL(model, TList<CollisionPrimitive>{});
			return vector_to_tlist(model->GetCollisionPrimitives());
		});
	}

	const CollisionMesh *Model_GetCollisionMesh(const Model *model) noexcept
	{
		return catchall([&] {
			CHECKNULL(model, static_cast<const CollisionMesh *>(nullptr));
			return model->GetCollisionMesh();
		});
	}

	// Scope //

	const Field *Scope_GetField(const Scope *scope, uint32_t name_hash) noexcept
	{
		return catchall([&] {
			CHECKNULL(scope, static_cast<const Field *>(nullptr));
			return scope->GetField(name_hash);
		});
	}

	TList<const Field *> Scope_GetFields(const Scope *scope, uint32_t name_hash) noexcept
	{
		return catchall([&] {
			CHECKNULL(scope, TList<const Field *>{});
			return vector_to_tlist(scope->GetFields(name_hash));
		});
	}

	// Segment //

	ETopology Segment_GetTopology(const Segment *seg) noexcept
	{
		return catchall([&] {
			CHECKNULL(seg, static_cast<ETopology>(0));
			return seg->GetTopology();
		});
	}

	size_t Segment_GetBoneName(const Segment *seg, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(seg, static_cast<size_t>(0));
			std::string bone_name = seg->GetBone();
			copy_c_str(bone_name, out_buffer, out_buffer_len);
			return bone_name.size();
		});
	}

	const Material *Segment_GetMaterial(const Segment *seg) noexcept
	{
		return catchall([&] {
			CHECKNULL(seg, static_cast<const Material *>(nullptr));
			return seg->GetMaterial();
		});
	}

	TList<uint16_t> Segment_GetIndexBuffer(const Segment *seg) noexcept
	{
		return catchall([&] {
			CHECKNULL(seg, TList<uint16_t>{});
			return vector_to_tlist(seg->GetIndexBuffer());
		});
	}

	TList<Vector3> Segment_GetVertexBuffer(const Segment *seg) noexcept
	{
		return catchall([&] {
			CHECKNULL(seg, TList<Vector3>{});
			return vector_to_tlist(seg->GetVertexBuffer());
		});
	}

	TList<Vector2> Segment_GetUVBuffer(const Segment *seg) noexcept
	{
		return catchall([&] {
			CHECKNULL(seg, TList<Vector2>{});
			return vector_to_tlist(seg->GetUVBuffer());
		});
	}

	TList<Vector3> Segment_GetNormalBuffer(const Segment *seg) noexcept
	{
		return catchall([&] {
			CHECKNULL(seg, TList<Vector3>{});
			return vector_to_tlist(seg->GetNormalBuffer());
		});
	}

	// Terrain //

	TList<uint32_t> Terrain_GetIndexBuffer(const Terrain *terr) noexcept
	{
		return catchall([&] {
			CHECKNULL(terr, TList<uint32_t>{});
			return vector_to_tlist(terr->GetIndexBuffer(ETopology::TriangleList));
		});
	}

	TList<Vector3> Terrain_GetVertexBuffer(const Terrain *terr) noexcept
	{
		return catchall([&] {
			CHECKNULL(terr, TList<Vector3>{});
			return vector_to_tlist(terr->GetVertexBuffer());
		});
	}

	TList<Vector2> Terrain_GetUVBuffer(const Terrain *terr) noexcept
	{
		return catchall([&] {
			CHECKNULL(terr, TList<Vector2>{});
			return vector_to_tlist(terr->GetUVBuffer());
		});
	}

	TList<uint8_t> Terrain_GetBlendMap(const Terrain *ter, uint32_t *out_dim, uint32_t *out_num_layers) noexcept
	{
		return catchall([&] {
			*out_dim = 0;
			*out_num_layers = 0;
			CHECKNULL(ter, TList<uint8_t>{});
			return vector_to_tlist(ter->GetBlendMap(*out_dim, *out_num_layers));
		});
	}

	TList<Texture> Terrain_GetLayerTextures(const Terrain *terr, const Container *container) noexcept
	{
		return catchall([&] {
			CHECKNULL(terr, TList<Texture>{});
			CHECKNULL(container, TList<Texture>{});
			std::vector<std::string> layer_texture_names = terr->GetLayerTextures();
			std::vector<Texture> layer_textures;
			for (const std::string &texture_name : layer_texture_names) {
				layer_textures.emplace_back(*container->FindTexture(texture_name));
			}
			return vector_to_tlist(layer_textures);
		});
	}

	// Texture //

	size_t Texture_GetName(const Texture *tex, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(tex, static_cast<size_t>(0));
			std::string name = tex->GetName();
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	TList<uint8_t> Texture_GetData(const Texture *tex, uint16_t *out_width, uint16_t *out_height) noexcept
	{
		return catchall([&] {
			if (tex == nullptr) {
				*out_width = 0;
				*out_height = 0;
				return TList<uint8_t>{};
			}
			const uint8_t *buff = nullptr;
			tex->GetImageData(ETextureFormat::R8_G8_B8_A8, 0, *out_width, *out_height, buff);
			size_t len = *out_width * *out_height * 4;
			return vector_to_tlist(std::vector<uint8_t>{buff, buff + len});
		});
	}

	// World //

	size_t World_GetName(const World *world, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(world, static_cast<size_t>(0));
			std::string name = world->GetName();
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	size_t World_GetSkyName(const World *world, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(world, static_cast<size_t>(0));
			std::string sky_name = world->GetSkyName();
			copy_c_str(sky_name, out_buffer, out_buffer_len);
			return sky_name.size();
		});
	}

	TList<Instance> World_GetInstances(const World *world) noexcept
	{
		return catchall([&] {
			CHECKNULL(world, TList<Instance>{});
			return vector_to_tlist(world->GetInstances());
		});
	}

	const Terrain *World_GetTerrain(const World *world) noexcept
	{
		return catchall([&] {
			CHECKNULL(world, static_cast<const Terrain *>(nullptr));
			return world->GetTerrain();
		});
	}

	size_t World_GetTerrainName(const World *world, char *out_buffer, size_t out_buffer_len) noexcept
	{
		return catchall([&] {
			CHECKNULL(world, static_cast<size_t>(0));
			std::string terrain_name = world->GetTerrainName();
			copy_c_str(terrain_name, out_buffer, out_buffer_len);
			return terrain_name.size();
		});
	}
}
