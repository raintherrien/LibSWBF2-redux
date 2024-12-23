#pragma once
#include "Types/Enums.h"
#include "Types/Vector2.h"
#include "Types/Vector3.h"
#include "Types/Vector4.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#include <functional>
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

	// A very basic list implementation with lifetime semantics, otherwise we're
	// passing pointers around owned by the library, statically allocated, etc. it's
	// just bad. Abuse C++ Standard Layout to supported a more user friendly
	// templated list for C++ consumers.
	struct CList;
	LIBSWBF2_API size_t CList_size(struct CList *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void *CList_at(struct CList *, size_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CList_free(struct CList *) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
} // extern "C"
#endif

// C++ gets a BARELY nicer typed CList wrapper
#ifdef __cplusplus
template<typename T>
class TList {
private:
	CList *impl;

	// This type punning is not a safe API. Use the static create function instead.
	TList(CList *impl_) noexcept
	: impl{impl_}
	{
	};

public:
	static TList<T> create(CList *impl_)
	{
		return {impl_};
	}

	~TList()
	{
		CList_free(impl);
	}

	T *at(size_t i) noexcept
	{
		if (!impl) {
			return nullptr;
		}
		return static_cast<T *>(CList_at(impl, i));
	}

	T *data() noexcept
	{
		if (!impl) {
			return nullptr;
		}
		return static_cast<T *>(CList_at(impl, 0));
	}

	size_t size() noexcept
	{
		if (!impl) {
			return 0;
		}
		return CList_size(impl);
	}

	operator bool() const noexcept
	{
		return impl != nullptr;
	}
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
	// Shared pointer wrappers //
	struct Container_Owned;
	struct Level_Owned;

	// Bone //
	LIBSWBF2_API struct Vector3 Bone_GetPosition(const struct Bone *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector4 Bone_GetRotation(const struct Bone *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Bone_GetName(const struct Bone *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Bone_GetParentName(const struct Bone *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API std::string Bone_GetNameS(const struct Bone *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API std::string Bone_GetParentNameS(const struct Bone *) LIBSWBF2_NOEXCEPT;
#endif

	// CollisionMesh //
	LIBSWBF2_API struct CList * /* uint16_t */ CollisionMesh_GetIndexBuffer(const struct CollisionMesh *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* Vector3 */ CollisionMesh_GetVertexBuffer(const struct CollisionMesh *) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API TList<uint16_t> CollisionMesh_GetIndexBufferT(const struct CollisionMesh *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<Vector3> CollisionMesh_GetVertexBufferT(const struct CollisionMesh *) LIBSWBF2_NOEXCEPT;
#endif

	// CollisionPrimitive //
	LIBSWBF2_API struct Vector3 CollisionPrimitive_GetPosition(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector4 CollisionPrimitive_GetRotation(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t CollisionPrimitive_GetParentName(const struct CollisionPrimitive *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API enum ECollisionPrimitiveType CollisionPrimitive_GetType(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionPrimitive_GetCubeDims(const struct CollisionPrimitive *, float *sx, float *sy, float *sz) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionPrimitive_GetCylinderDims(const struct CollisionPrimitive *, float *sr, float *sh) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionPrimitive_GetSphereRadius(const struct CollisionPrimitive *, float *sr) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API std::string CollisionPrimitive_GetParentNameS(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
#endif

	// Config //
	LIBSWBF2_API const struct Field *Config_GetField(const struct Config *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Field * */ Config_GetFields(const struct Config *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API TList<const struct Field *> Config_GetFieldsT(const struct Config *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
#endif

	// Container //
	LIBSWBF2_API struct Container_Owned *Container_Create() LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Container_Destroy(struct Container_Owned *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Level_Owned *Container_AddLevel(const struct Container_Owned *, const char *lvl_path) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Config *Container_FindConfig(const struct Container_Owned *, enum EConfigType, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Texture *Container_FindTexture(const struct Container_Owned *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Model *Container_FindModel(const struct Container_Owned *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct EntityClass *Container_FindEntityClass(const struct Container_Owned *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;

	// EntityClass //
	LIBSWBF2_API size_t EntityClass_GetBaseName(const struct EntityClass *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t EntityClass_GetPropertyValue(const struct EntityClass *, uint32_t name_hash, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* uint32_t */ EntityClass_GetAllPropertyHashes(const struct EntityClass *) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API std::string EntityClass_GetBaseNameS(const struct EntityClass *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API std::string EntityClass_GetPropertyValueS(const struct EntityClass *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<uint32_t> EntityClass_GetAllPropertyHashesT(const struct EntityClass *) LIBSWBF2_NOEXCEPT;
#endif

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
#ifdef __cplusplus
	LIBSWBF2_API std::string Instance_GetNameS(const struct Instance *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API std::string Instance_GetEntityClassNameS(const struct Instance *) LIBSWBF2_NOEXCEPT;
#endif

	// Level //
	LIBSWBF2_API void Level_Destroy(struct Level_Owned *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool Level_IsWorldLevel(const struct Level_Owned *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Level_GetName(const struct Level_Owned *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct World */ Level_GetWorlds(const struct Level_Owned *) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API TList<const struct World> Level_GetWorldsT(const struct Level_Owned *) LIBSWBF2_NOEXCEPT;
#endif

	// Material //
	LIBSWBF2_API const struct Texture *Material_GetTexture(const struct Material *, size_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API enum EMaterialFlags Material_GetFlags(const struct Material *) LIBSWBF2_NOEXCEPT;

	// Model //
	LIBSWBF2_API const struct CollisionMesh *Model_GetCollisionMesh(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Segment */ Model_GetSegments(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Bone */ Model_GetBones(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct CollisionPrimitive */ Model_GetCollisionPrimitives(const struct Model *) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API TList<const struct Segment> Model_GetSegmentsT(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<const struct Bone> Model_GetBonesT(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<const struct CollisionPrimitive> Model_GetCollisionPrimitivesT(const struct Model *) LIBSWBF2_NOEXCEPT;
#endif

	// Scope //
	LIBSWBF2_API const struct Field *Scope_GetField(const struct Scope *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Field * */ Scope_GetFields(const struct Scope *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API TList<const struct Field *> Scope_GetFieldsT(const struct Scope *, uint32_t name_hash) LIBSWBF2_NOEXCEPT;
#endif

	// Segment //
	LIBSWBF2_API enum ETopology Segment_GetTopology(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Segment_GetBoneName(const struct Segment *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Material *Segment_GetMaterial(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* uint16_t */ Segment_GetIndexBuffer(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Vector3 */ Segment_GetVertexBuffer(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Vector2 */ Segment_GetUVBuffer(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Vector3 */ Segment_GetNormalBuffer(const struct Segment *) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API std::string Segment_GetBoneNameS(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<uint16_t> Segment_GetIndexBufferT(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<const struct Vector3> Segment_GetVertexBufferT(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<const struct Vector2> Segment_GetUVBufferT(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<const struct Vector3> Segment_GetNormalBufferT(const struct Segment *) LIBSWBF2_NOEXCEPT;
#endif

	// Terrain //
	LIBSWBF2_API struct CList * /* uint32_t */ Terrain_GetIndexBuffer(const struct Terrain *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Vector3 */ Terrain_GetVertexBuffer(const struct Terrain *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Vector2 */ Terrain_GetUVBuffer(const struct Terrain *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* uint8_t */ Terrain_GetBlendMap(const struct Terrain *, uint32_t *out_width, uint32_t *out_num_layers) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /*const struct Texture */ Terrain_GetLayerTextures(const struct Terrain *, const struct Container_Owned *) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API TList<uint32_t> Terrain_GetIndexBufferT(const struct Terrain *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<const struct Vector3> Terrain_GetVertexBufferT(const struct Terrain *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<const struct Vector2> Terrain_GetUVBufferT(const struct Terrain *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<uint8_t> Terrain_GetBlendMapT(const struct Terrain *, uint32_t *out_width, uint32_t *out_num_layers) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<const struct Texture> Terrain_GetLayerTexturesT(const struct Terrain *, const struct Container_Owned *) LIBSWBF2_NOEXCEPT;
#endif

	// Texture //
	LIBSWBF2_API size_t Texture_GetName(const struct Texture *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* uint8_t */ Texture_GetData(const struct Texture *, uint16_t *out_width, uint16_t *out_height) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API std::string Texture_GetNameS(const struct Texture *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<uint8_t> Texture_GetDataT(const struct Texture *, uint16_t *out_width, uint16_t *out_height) LIBSWBF2_NOEXCEPT;
#endif

	// World //
	LIBSWBF2_API size_t World_GetName(const struct World *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t World_GetSkyName(const struct World *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Terrain *World_GetTerrain(const struct World *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t World_GetTerrainName(const struct World *, char *out_buffer, size_t out_buffer_len) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct CList * /* const struct Instance */ World_GetInstances(const struct World *) LIBSWBF2_NOEXCEPT;
#ifdef __cplusplus
	LIBSWBF2_API std::string World_GetNameS(const struct World *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API TList<const struct Instance> World_GetInstancesT(const struct World *) LIBSWBF2_NOEXCEPT;
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
} // namespace LibSWBF2
#endif
