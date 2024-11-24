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
	struct Connection;
	struct Hub;
	struct Vector2;
	struct Vector3;
	struct Vector4;
#ifdef __cplusplus
} // namespace Types
#endif

#ifdef __cplusplus
namespace Wrappers {
#endif
	struct AnimationBank;
	struct Bone;
	struct CollisionMesh;
	struct CollisionPrimitive;
	struct Config;
	struct EntityClass;
	struct Field;
	struct Instance;
	struct Level;
	struct Localization;
	struct Material;
	struct Model;
	struct Scope;
	struct Segment;
	struct Sound;
	struct SoundBank;
	struct SoundStream;
	struct Terrain;
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
// # A note on strings and arrays
// I use sprintf-style for returning variable length data from functions, where
// calling a string returning function with a NULL buffer will return the number
// of characters that would have been written to the buffer EXCLUDING the null
// terminating character.
#ifdef __cplusplus
extern "C" {
#endif
	// Helpers //
	LIBSWBF2_API uint32_t FNVHashString(const char *string) LIBSWBF2_NOEXCEPT;

// XXX Everything below this line is a work in progress
#if 0
	// AnimationBank //
	LIBSWBF2_API bool AnimationBank_GetAnimationMetadata(const struct AnimationBank *setPtr, uint32_t animCRC, int32_t *out_numFrames, int32_t *out_numBones) LIBSWBF2_NOEXCEPT;

	// Bone //
	LIBSWBF2_API const char *Bone_GetName(const struct Bone *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Bone_GetParentName(const struct Bone *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector3 Bone_GetPosition(const struct Bone *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector4 Bone_GetRotation(const struct Bone *) LIBSWBF2_NOEXCEPT;

	// CollisionMesh //
	LIBSWBF2_API void CollisionMesh_GetIndexBuffer(const struct CollisionMesh *, uint16_t **out_indices, uint32_t *out_numInds) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionMesh_GetVertexBuffer(const struct CollisionMesh *, struct Vector3 **out_verts, uint32_t *out_numVerts) LIBSWBF2_NOEXCEPT;

	// CollisionPrimitive //
	LIBSWBF2_API const char *CollisionPrimitive_GetParentName(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector3 CollisionPrimitive_GetPosition(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector4 CollisionPrimitive_GetRotation(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API enum ECollisionPrimitiveType CollisionPrimitive_GetType(const struct CollisionPrimitive *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionPrimitive_GetCubeDims(const struct CollisionPrimitive *, float *sx, float *sy, float *sz) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionPrimitive_GetCylinderDims(const struct CollisionPrimitive *, float *sr, float *sh) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void CollisionPrimitive_GetSphereRadius(const struct CollisionPrimitive *, float *sr) LIBSWBF2_NOEXCEPT;

	// Config //
	LIBSWBF2_API const struct Field *Config_GetField(const struct Config *, uint32_t hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Field **Config_GetFields(const struct Config *, uint32_t hash, size_t *out_fieldCount) LIBSWBF2_NOEXCEPT;

	// Connection //
	LIBSWBF2_API const void *Connection_GetFieldPtr(const struct Connection *con, uint8_t id) LIBSWBF2_NOEXCEPT;

	// Container //
	LIBSWBF2_API struct Container *Container_Initialize() LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Level *Container_AddLevel(struct Container *container, const char *path) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Level *Container_GetLevel(struct Container *container, uint32_t handle) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool Container_Delete(struct Container *container) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Config *Container_FindConfig(struct Container *container, enum EConfigType type, uint32_t nameHash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Texture *Container_FindTexture(struct Container *container, uint32_t nameHash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Model *Container_FindModel(struct Container *container, uint32_t nameHash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct EntityClass *Container_FindEntityClass(struct Container *container, uint32_t nameHash) LIBSWBF2_NOEXCEPT;

	// EntityClass //
	LIBSWBF2_API const char *EntityClass_GetName(const struct EntityClass *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct EntityClass *EntityClass_GetBase(const struct EntityClass *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *EntityClass_GetBaseName(const struct EntityClass *) LIBSWBF2_NOEXCEPT;

	// Field //
	LIBSWBF2_API uint32_t Field_GetNameHash(const struct Field *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Field_GetNumValues(const struct Field *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API float Field_GetFloat(const struct Field *, uint8_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint32_t Field_GetUInt32(const struct Field *, uint8_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Field_GetString(const struct Field *, uint8_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Field_GetName(const struct Field *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Scope *Field_GetScope(const struct Field *) LIBSWBF2_NOEXCEPT;

	// Hub //
	LIBSWBF2_API const void *Hub_GetFieldPtr(const struct Hub *hub, uint8_t id, int32_t *out_numBytes) LIBSWBF2_NOEXCEPT;

	// Instance //
	LIBSWBF2_API const char *Instance_GetName(const struct Instance *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Instance_GetEntityClassName(const struct Instance *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector3 Instance_GetPosition(const struct Instance *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Vector4 Instance_GetRotation(const struct Instance *) LIBSWBF2_NOEXCEPT;

	// Level //
	LIBSWBF2_API uint8_t Level_IsWorldLevel(const struct Level *level) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Level_GetName(const struct Level *level) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Config *Level_GetConfig(const struct Level *level, uint32_t header, uint32_t hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct SoundStream *Level_FindAndIndexSoundStream(struct Level *level, struct FileReader *stream, uint32_t StreamName) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Level_GetWorldCount(const struct Level *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct World *Level_GetWorld(const struct Level *, size_t index) LIBSWBF2_NOEXCEPT;

	// Localization //
	LIBSWBF2_API const char *Localization_GetName(const struct Localization *local) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Localization_GetLocalizedWideString(const struct Localization *local, const char *path, uint16_t **out_chars, uint32_t *out_count) LIBSWBF2_NOEXCEPT;

	// Logging //
	LIBSWBF2_API void LOG_SetLogfileLevel(enum ELogType LogfileLevel) LIBSWBF2_NOEXCEPT;

	// Material //
	LIBSWBF2_API const struct Texture *Material_GetTexture(const struct Material *, size_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API enum EMaterialFlags Material_GetFlags(const struct Material *) LIBSWBF2_NOEXCEPT;

	// Memory //
	LIBSWBF2_API void Memory_Blit(void *dest, void *src, int numBytes) LIBSWBF2_NOEXCEPT;

	// Model //
	LIBSWBF2_API size_t Model_GetSegmentCount(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Segment *Model_GetSegment(const struct Model *, size_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Model_GetBoneCount(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Bone *Model_GetBone(const struct Model *, size_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct CollisionMesh *Model_GetCollisionMesh(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Model_GetCollisionPrimitiveCount(const struct Model *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct CollisionPrimitive *Model_GetCollisionPrimitive(const struct Model *, size_t index) LIBSWBF2_NOEXCEPT;

	// Scope //
	LIBSWBF2_API const struct Field *Scope_GetField(const struct Scope *, uint32_t hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Field **Scope_GetFields(const struct Scope *, uint32_t hash, size_t *out_fieldCount) LIBSWBF2_NOEXCEPT;

	// Segment //
	LIBSWBF2_API void Segment_GetIndexBuffer(const struct Segment *, uint16_t **out_indices, uint32_t *out_numInds) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Segment_GetVertexBuffer(const struct Segment *, struct Vector3 **out_verts, uint32_t *out_numVerts) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Segment_GetUVBuffer(const struct Segment *, struct Vector2 **out_uvs, uint32_t *out_numUVs) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Segment_GetNormalBuffer(const struct Segment *, struct Vector3 **out_normals, uint32_t *out_numNormals) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API enum ETopology Segment_GetTopology(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Material *Segment_GetMaterial(const struct Segment *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Segment_GetBoneName(const struct Segment *) LIBSWBF2_NOEXCEPT;

	// Sound //
	LIBSWBF2_API uint8_t Sound_GetData(const struct Sound *sound, uint32_t *out_sampleRate, uint32_t *out_sampleCount, uint8_t *out_blockAlign, const uint8_t **out_data) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Sound_FillDataBuffer(const struct Sound *sound, void *buffer) LIBSWBF2_NOEXCEPT;

	// SoundBank //
	LIBSWBF2_API uint8_t SoundBank_GetSound(const struct SoundBank *str, uint32_t soundName, const struct Sound* *out_sound) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t SoundBank_GetSounds(const struct SoundBank *str, const struct Sound **out_sounds, uint32_t *out_numSounds, uint32_t *out_soundInc) LIBSWBF2_NOEXCEPT;

	// SoundStream //
	LIBSWBF2_API uint8_t SoundStream_GetSound(const struct SoundStream *str, uint32_t soundName, const struct Sound **out_sound) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t SoundStream_GetSounds(const struct SoundStream *str, const struct Sound **out_sounds, uint32_t *out_numSounds, uint32_t *out_soundInc) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API int32_t SoundStream_SampleReadMethod(struct SoundStream *str, void *sBuf, int32_t sBufLength, int32_t numToRead, enum ESoundFormat format, int32_t *out_numBytesRead, bool ReadSamples) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool SoundStream_SetFileReader(struct SoundStream *str, struct FileReader *readerPtr) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool SoundStream_SetStreamBuffer(struct SoundStream *str, void *bufferPtr, int32_t bufferLgenth) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API int32_t SoundStream_ReadBytesFromStream(struct SoundStream *str, int32_t NumBytes) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API int32_t SoundStream_GetNumSamplesInBytes(struct SoundStream *str, int32_t NumBytes) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool SoundStream_SetSegment(struct SoundStream *str, const char *name) LIBSWBF2_NOEXCEPT;

	// Terrain //
	LIBSWBF2_API void Terrain_GetHeightMap(const struct Terrain *ter, uint32_t *out_dim, uint32_t *out_dimScale, float **out_heightData) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Terrain_GetBlendMap(const struct Terrain *ter, uint32_t *out_width, uint32_t *out_numLayers, uint8_t **out_data) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Terrain_GetIndexBuffer(const struct Terrain *terr, uint32_t **out_indices, uint32_t *out_numInds) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Terrain_GetVertexBuffer(const struct Terrain *terr, struct Vector3 **out_verts, uint32_t *out_numVerts) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Terrain_GetUVBuffer(const struct Terrain *terr, struct Vector2 **out_uvs, uint32_t *out_numUVs) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t Terrain_GetLayerTextureCount(const struct Terrain *terr) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Terrain_GetLayerTextureName(const struct Terrain *terr, size_t index) LIBSWBF2_NOEXCEPT;

	// World //
	LIBSWBF2_API bool World_GetChildrenList(const struct World *, uint8_t listID, void **out_listPtr, int32_t *out_listCount, int32_t *out_wrapperSize) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *World_GetName(const struct World *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *World_GetSkyName(const struct World *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Terrain *World_GetTerrain(const struct World *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *World_GetTerrainName(const struct World *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API size_t World_GetInstanceCount(const struct World *) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Instance *World_GetInstance(const struct World *, size_t index) LIBSWBF2_NOEXCEPT;
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
} // namespace LibSWBF2
#endif
