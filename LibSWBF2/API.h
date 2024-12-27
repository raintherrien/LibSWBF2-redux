#pragma once
#include "Types/Enums.h"
#include "Types/Vector2.h"
#include "Types/Vector3.h"
#include "Types/Vector4.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#define LIBSWBF2_NOEXCEPT noexcept
#define LIBSWBF2_DEPRECATED [[deprecated]]
#else
#define LIBSWBF2_NOEXCEPT
#define LIBSWBF2_DEPRECATED
#endif

#ifdef __cplusplus
namespace LibSWBF2 {
#endif

struct Container;

#ifdef __cplusplus
namespace Types {
#endif
	struct Vector2;
	struct Vector3;
	struct Vector4;
#ifdef __cplusplus
} // namespace Types
#endif

#ifdef __cplusplus
namespace Wrappers {
#endif
	struct Bone;
	struct CollisionMesh;
	struct CollisionPrimitive;
	struct Config;
	struct EntityClass;
	struct Field;
	struct Instance;
	struct Level;
	struct Material;
	struct Model;
	struct Scope;
	struct Segment;
	struct Terrain;
	struct Texture;
	struct World;
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
using namespace Types;
using namespace Wrappers;
#endif

// A very basic list implementation with lifetime semantics, otherwise we're
// passing pointers around owned by the library, statically allocated, etc. it's
// just bad. Abuse C++ Standard Layout to supported a more user friendly
// templated list for C++ consumers.
#ifdef __cplusplus
extern "C" {
#endif
struct CList {
	void *elements;
	size_t size;
	size_t element_size;
};

LIBSWBF2_API void CList_free(struct CList *) LIBSWBF2_NOEXCEPT;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#define LIBSWBF2_APILIST(T) TList<T>
template<typename T>
struct TList : CList{
	TList() noexcept
	{
		elements = nullptr;
		size = 0;
		element_size = sizeof(T);
	}

	TList(void *elements_, size_t size_, size_t element_size_) noexcept
	{
		elements = elements_;
		size = size_;
		element_size = element_size_;
	}

	~TList() noexcept
	{
		CList_free(this);
	}

	T &operator[](size_t i)
	{
		return *reinterpret_cast<T *>(reinterpret_cast<char *>(elements) + i * element_size);
	}

	const T &operator[](size_t i) const
	{
		return *reinterpret_cast<T *>(reinterpret_cast<char *>(elements) + i * element_size);
	}
};

#else

#define LIBSWBF2_APILIST(T) struct CList

#endif

// C API which will never throw an exception and does its damndest to not return
// any data subject to hidden lifetimes.
//
// # A note on strings
// I use sprintf-style for returning variable length data from functions, where
// calling a string returning function with a NULL buffer will return the number
// of characters that would have been written to the buffer EXCLUDING the null
// terminating character.
#ifdef __cplusplus
extern "C" {
#endif
	// Bone //
	LIBSWBF2_API struct Vector3 Bone_GetPosition(const struct Bone *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector4 Bone_GetRotation(const struct Bone *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Bone_GetName(const struct Bone *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Bone_GetParentName(const struct Bone *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;

	// CollisionMesh //
	LIBSWBF2_API LIBSWBF2_APILIST(uint16_t) CollisionMesh_GetIndexBuffer(const struct CollisionMesh *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(Vector3) CollisionMesh_GetVertexBuffer(const struct CollisionMesh *) LIBSWBF2_NOEXCEPT;

	// CollisionPrimitive //
	LIBSWBF2_API struct Vector3 CollisionPrimitive_GetPosition(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector4 CollisionPrimitive_GetRotation(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t CollisionPrimitive_GetParentName(const struct CollisionPrimitive *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API enum ECollisionPrimitiveType CollisionPrimitive_GetType(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionPrimitive_GetCubeDims(const struct CollisionPrimitive *, float *sx, float *sy, float *sz) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionPrimitive_GetCylinderDims(const struct CollisionPrimitive *, float *sr, float *sh) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionPrimitive_GetSphereRadius(const struct CollisionPrimitive *, float *sr) LIBSWBF2_NOEXCEPT;

	// Config //
	LIBSWBF2_API const struct Field *Config_GetField(const struct Config *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(const Field *) Config_GetFields(const struct Config *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;

	// Container //
	LIBSWBF2_API struct Container *Container_Create() LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Container_Destroy(struct Container *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Level *Container_AddLevel(struct Container *, const char *lvl_path) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Config *Container_FindConfig(struct Container *, enum EConfigType, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Texture *Container_FindTexture(struct Container *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Model *Container_FindModel(struct Container *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct EntityClass *Container_FindEntityClass(struct Container *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;

	// EntityClass //
	LIBSWBF2_API size_t EntityClass_GetBaseName(const struct EntityClass *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(uint32_t) EntityClass_GetAllPropertyHashes(const struct EntityClass *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t EntityClass_GetPropertyValue(const struct EntityClass *, uint32_t name_hash, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;

	// Field //
	LIBSWBF2_API const struct Scope *Field_GetScope(const struct Field *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Field_GetString(const struct Field *, size_t index, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;

	// Helpers //
	LIBSWBF2_API uint32_t FNVHashString(const char *string) LIBSWBF2_NOEXCEPT;

	// Instance //
	LIBSWBF2_API size_t Instance_GetName(const struct Instance *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Instance_GetEntityClassName(const struct Instance *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector3 Instance_GetPosition(const struct Instance *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector4 Instance_GetRotation(const struct Instance *) LIBSWBF2_NOEXCEPT;

	// Level //
	LIBSWBF2_API LIBSWBF2_APILIST(World) Level_GetWorlds(const struct Level *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool Level_IsWorldLevel(const struct Level *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Level_GetName(const struct Level *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;

	// Material //
	LIBSWBF2_API const struct Texture *Material_GetTexture(const struct Material *, size_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API enum EMaterialFlags Material_GetFlags(const struct Material *) LIBSWBF2_NOEXCEPT;

	// Model //
	LIBSWBF2_API LIBSWBF2_APILIST(Segment) Model_GetSegments(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(Bone) Model_GetBones(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(CollisionPrimitive) Model_GetCollisionPrimitives(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct CollisionMesh *Model_GetCollisionMesh(const struct Model *) LIBSWBF2_NOEXCEPT;

	// Scope //
	LIBSWBF2_API const struct Field *Scope_GetField(const struct Scope *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(const Field *) Scope_GetFields(const struct Scope *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;

	// Segment //
	LIBSWBF2_API enum ETopology Segment_GetTopology(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Segment_GetBoneName(const struct Segment *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Material *Segment_GetMaterial(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(uint16_t) Segment_GetIndexBuffer(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(Vector3) Segment_GetVertexBuffer(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(Vector2) Segment_GetUVBuffer(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(Vector3) Segment_GetNormalBuffer(const struct Segment *) LIBSWBF2_NOEXCEPT;

	// Terrain //
	LIBSWBF2_API LIBSWBF2_APILIST(uint32_t) Terrain_GetIndexBuffer(const struct Terrain *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(Vector3) Terrain_GetVertexBuffer(const struct Terrain *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(Vector2) Terrain_GetUVBuffer(const struct Terrain *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(uint8_t) Terrain_GetBlendMap(const struct Terrain *, uint32_t *out_width, uint32_t *out_num_layers) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(Texture) Terrain_GetLayerTextures(const struct Terrain *, const struct Container *) LIBSWBF2_NOEXCEPT;

	// Texture //
	LIBSWBF2_API size_t Texture_GetName(const struct Texture *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(uint8_t) Texture_GetData(const struct Texture *, uint16_t *out_width, uint16_t *out_height) LIBSWBF2_NOEXCEPT;

	// World //
	LIBSWBF2_API size_t World_GetName(const struct World *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t World_GetSkyName(const struct World *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API LIBSWBF2_APILIST(Instance) World_GetInstances(const struct World *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Terrain *World_GetTerrain(const struct World *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t World_GetTerrainName(const struct World *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
} // namespace LibSWBF2
#endif
