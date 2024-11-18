#pragma once
#include "Types/Enums.h"
#include <stdbool.h>

#ifdef __cplusplus
#define LIBSWBF2_NOEXCEPT noexcept
#else
#define LIBSWBF2_NOEXCEPT
#endif

#ifdef __cplusplus
namespace LibSWBF2 {
#endif

struct Container;

#ifdef __cplusplus
namespace Chunks::MSH {
#endif
	struct MSH;
	struct MODL;
	struct STRP;
#ifdef __cplusplus
} // namespace Chunks::MSH
#endif

#ifdef __cplusplus
namespace Types {
#endif
	struct Connection;
	struct Hub;
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct WorldAnimationKey;
#ifdef __cplusplus
} // namespace Types
#endif

#ifdef __cplusplus
namespace Wrappers {
#endif
	struct AnimationBank;
	struct AnimationSkeleton;
	struct Barrier;
	struct Bone;
	struct CollisionMesh;
	struct CollisionPrimitive;
	struct Config;
	struct EntityClass;
	struct Field;
	struct HintNode;
	struct Instance;
	struct Joint;
	struct Level;
	struct Localization;
	struct Material;
	struct Model;
	struct PlanSet;
	struct Region;
	struct Scope;
	struct Script;
	struct Segment;
	struct Sound;
	struct SoundBank;
	struct SoundStream;
	struct Terrain;
	struct Texture;
	struct VertexWeight;
	struct World;
	struct WorldAnimation;
	struct WorldAnimationGroup;
	struct WorldAnimationHierarchy;
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
using namespace Chunks::MSH;
using namespace Types;
using namespace Wrappers;
#endif

// Provide mangling free C-functions to be accessible from C# wrapper
#ifdef __cplusplus
extern "C" {
#endif

	// AnimationBank //
	LIBSWBF2_API bool AnimationBank_GetCurve(const struct AnimationBank *setPtr, uint32_t animCRC, uint32_t boneCRC, uint32_t comp, const uint16_t **out_indicesBuffer, const float **out_valuesBuffer, int32_t *out_numKeys) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const uint32_t *AnimationBank_GetAnimationCRCs(const struct AnimationBank *setPtr, int32_t *out_numCRCs) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const uint32_t *AnimationBank_GetBoneCRCs(const struct AnimationBank *setPtr, uint32_t animCRC, int32_t *out_numCRCs) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool AnimationBank_GetAnimationMetadata(const struct AnimationBank *setPtr, uint32_t animCRC, int32_t *out_numFrames, int32_t *out_numBones) LIBSWBF2_NOEXCEPT;

	// AnimationSkeleton //
	LIBSWBF2_API const char *AnimationSkeleton_GetName(const struct AnimationSkeleton *skelPtr) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t AnimationSkeleton_GetJoints(const struct AnimationSkeleton *skelPtr, int32_t *out_numJoints, struct Joint **out_joints) LIBSWBF2_NOEXCEPT;

	// Barrier //
	LIBSWBF2_API const void *Barrier_GetFieldPtr(const struct Barrier *bar, uint8_t fieldID) LIBSWBF2_NOEXCEPT;

	// Bone //
	LIBSWBF2_API void Bone_FetchAllFields(const struct Bone *bone, const char **out_name, const char **out_parentName, const Vector3 **out_loc, const Vector4 **out_rot) LIBSWBF2_NOEXCEPT;

	// CollisionMesh //
	LIBSWBF2_API uint8_t CollisionMesh_FetchAllFields(const struct CollisionMesh *cmPtr, uint32_t *out_iCount, uint16_t **out_iBuf, uint32_t *out_vCount, struct Vector3 **out_vBuf, uint32_t *out_maskFlags, const char **out_namePtr, const char **out_nodeNamePtr) LIBSWBF2_NOEXCEPT;

	// CollisionPrimitive //
	LIBSWBF2_API void CollisionPrimitive_FetchAllFields(struct CollisionPrimitive *primPtr, float *out_f1, float *out_f2, float *out_f3, const char **out_name, const char **out_parentName, uint32_t *out_maskFlags, uint32_t *out_primitiveType, Vector3 **out_pos, Vector4 **out_rot) LIBSWBF2_NOEXCEPT;

	// Config //
	LIBSWBF2_API uint8_t Config_FetchSimpleFields(const struct Config *cfg, uint32_t *out_name) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Field **Config_GetFields(struct Config *, uint32_t hash, uint32_t *out_count) LIBSWBF2_NOEXCEPT;

	// Connection //
	LIBSWBF2_API const void *Connection_GetFieldPtr(const struct Connection *con, uint8_t id) LIBSWBF2_NOEXCEPT;

	// Container //
	LIBSWBF2_API const struct Container *Container_Initialize() LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint16_t Container_AddLevel(struct Container *container, const char *path) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint16_t Container_AddLevelFiltered(struct Container *container, const char *path, const char **subLVLs, uint32_t subLVLCount) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Container_FreeAll(struct Container *container, uint8_t force) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API float Container_GetProgress(struct Container *container, uint32_t handle) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API float Container_GetOverallProgress(struct Container *container) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Level *Container_GetLevel(struct Container *container, uint32_t handle) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Container_GetLoadedLevels(struct Container *container, uint16_t **out_handles, uint16_t handleCount) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Container_GetStatus(struct Container *container, uint32_t handle) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Container_LoadLevels(struct Container *container) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool Container_IsDone(struct Container *container) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool Container_Delete(struct Container *container) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const void *Container_GetWrapper(struct Container *container, uint32_t type, const char *name) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const void *Container_GetWrapperFNV(struct Container *container, uint32_t type, uint32_t name) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Config *Container_GetConfig(struct Container *container, uint32_t type, uint32_t nameHash) LIBSWBF2_NOEXCEPT;

	// EntityClass //
	LIBSWBF2_API void EntityClass_FetchAllFields(const struct EntityClass *ec, const char **out_name, uint8_t *out_classType, const struct EntityClass **out_baseClass, const char **out_baseClassName) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t EntityClass_GetPropertyFromName(const struct EntityClass *ec, const char *propName, const char **out_value) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t EntityClass_GetPropertyFromHash(const struct EntityClass *ec, uint32_t hashedPropName, const char **out_value) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t EntityClass_GetPropertiesFromName(const struct EntityClass *ec, const char *propName, const char ***out_values, uint32_t *out_count) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t EntityClass_GetPropertiesFromHash(const struct EntityClass *ec, uint32_t hashedPropName, const char ***out_values, uint32_t *out_count) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void EntityClass_GetOverriddenProperties(const struct EntityClass *ec, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void EntityClass_GetAllProperties(const struct EntityClass *ec, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) LIBSWBF2_NOEXCEPT;

	// Enum //
	LIBSWBF2_API const char *ENUM_TopologyToString(enum ETopology topology) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *ENUM_MaterialFlagsToString(enum EMaterialFlags flags) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *ENUM_EVBUFFlagsToString(enum EVBUFFlags flags) LIBSWBF2_NOEXCEPT;

	// Field //
	LIBSWBF2_API uint8_t Field_FetchAllFields(const struct Field *cfg, struct Scope **out_scop) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint32_t Field_GetNameHash(const struct Field *cfg) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Field_GetNumValues(const struct Field *cfg) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API float Field_GetFloat(const struct Field *cfg, uint8_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint32_t Field_GetUInt32(const struct Field *cfg, uint8_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Vector2 *Field_GetVec2(const struct Field *cfg) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Vector3 *Field_GetVec3(const struct Field *cfg) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Vector4 *Field_GetVec4(const struct Field *cfg) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Field_GetString(const struct Field *cfg, uint8_t index) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Field_GetName(const struct Field *cfg) LIBSWBF2_NOEXCEPT;

	// FileReader //
	LIBSWBF2_API const struct FileReader *FileReader_FromFile(const char *path, bool UseMemoryMapping) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void FileReader_Delete(struct FileReader *readerPtr) LIBSWBF2_NOEXCEPT;

	// Hash Lookup //
	LIBSWBF2_API uint8_t Hashing_Lookup(uint32_t hash, const char **out_str) LIBSWBF2_NOEXCEPT;

	// HintNode //
	LIBSWBF2_API const void *HintNode_GetFieldPtr(const struct HintNode *hnt, uint8_t fieldID) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void HintNode_GetProperties(const struct HintNode *hnt, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) LIBSWBF2_NOEXCEPT;

	// Hub //
	LIBSWBF2_API const void *Hub_GetFieldPtr(const struct Hub *hub, uint8_t id, int32_t *out_numBytes) LIBSWBF2_NOEXCEPT;

	// Instance //
	LIBSWBF2_API uint8_t Instance_FetchSimpleFields(const struct Instance *instPtr, const char **out_name, struct Vector4 **out_rot, struct Vector3 **out_pos, const char **out_ecName, const struct EntityClass **out_ec) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Instance_GetPropertyFromName(const struct Instance *instPtr, const char *propName, const char **out_value) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Instance_GetPropertyFromHash(const struct Instance *instPtr, uint32_t hashedPropName, const char **out_value) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Instance_GetPropertiesFromName(const struct Instance *instPtr, const char *propName, const char ***out_values, uint32_t *out_count) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Instance_GetPropertiesFromHash(const struct Instance *instPtr, uint32_t hashedPropName, const char ***out_values, uint32_t *out_count) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Instance_GetOverriddenProperties(const struct Instance *instPtr, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) LIBSWBF2_NOEXCEPT;

	// Level //
	LIBSWBF2_API struct Level *Level_FromFile(const char *path) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct Level *Level_FromStream(struct FileReader *stream) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Level_Destroy(struct Level *level) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Level_IsWorldLevel(const struct Level *level) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const char *Level_GetName(const struct Level *level) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Config *Level_GetConfig(const struct Level *level, uint32_t header, uint32_t hash) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const struct Config **Level_GetConfigs(const struct Level *level, uint32_t header, int32_t *out_numConfigs) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const void *Level_GetWrapper(const struct Level *level, uint32_t type, const char *name) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const void *Level_GetWrapperFNV(const struct Level *level, uint32_t type, uint32_t name) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API const void *Level_GetWrappers(const struct Level *level, uint32_t type, uint32_t *out_numWrappers, uint32_t *out_wrapperSize) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API struct SoundStream *Level_FindAndIndexSoundStream(struct Level *level, struct FileReader *stream, uint32_t StreamName) LIBSWBF2_NOEXCEPT;

	// Localization //
	LIBSWBF2_API const char *Localization_GetName(const struct Localization *local) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Localization_GetLocalizedWideString(const struct Localization *local, const char *path, uint16_t **out_chars, uint32_t *out_count) LIBSWBF2_NOEXCEPT;

	// Logging //
	LIBSWBF2_API uint8_t LOG_GetNextLog(const char **out_msg, enum ELogType *out_level, uint32_t *out_line, const char **out_file) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void LOG_SetLogfileLevel(enum ELogType LogfileLevel) LIBSWBF2_NOEXCEPT;

	// Material //
	LIBSWBF2_API uint8_t Material_FetchAllFields(const struct Material *matPtr, struct Vector3 **out_specular, struct Vector3 **out_diffuse, char ***out_texPtrs, int32_t *out_numTexes, char **out_attachedLightName, uint32_t *out_matFlags, uint32_t *out_specExp, uint32_t *out_param1, uint32_t *out_param2) LIBSWBF2_NOEXCEPT;

	// Memory //
	LIBSWBF2_API void Memory_Blit(void *dest, void *src, int numBytes) LIBSWBF2_NOEXCEPT;

	// Model //
	LIBSWBF2_API uint8_t Model_FetchSimpleFields(const struct Model *model, const char **out_name, uint8_t *out_skinned, uint8_t *out_skelBroken, const struct Segment **out_segArr, int32_t *out_segCount, int32_t *out_segInc, const Bone **out_boneArr, int32_t *out_boneCount, int32_t *out_boneInc, const struct CollisionMesh **out_collMeshPtr) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Model_GetPrimitivesMasked(const struct Model *model, uint32_t mask, int *out_numPrims, CollisionPrimitive ***out_primArrayPtr) LIBSWBF2_NOEXCEPT;

	// MODL //
	LIBSWBF2_API enum EModelPurpose MODL_GetPurpose(struct MODL *modl) LIBSWBF2_NOEXCEPT;

	// MSH //
	LIBSWBF2_API struct MSH *MSH_Create() LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t MSH_Delete(struct MSH *msh) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t MSH_ReadFromFile(struct MSH *msh, const char *path) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t MSH_WriteToFile(struct MSH *msh, const char *path) LIBSWBF2_NOEXCEPT;

	// PlanSet //
	LIBSWBF2_API uint8_t PlanSet_GetChildWrappers(const struct PlanSet *ps, uint8_t id, void **out_listPtr, int32_t *out_listSize, int32_t *out_elSize) LIBSWBF2_NOEXCEPT;

	// Region //
	LIBSWBF2_API uint8_t Region_FetchAllFields(const struct Region *reg, const struct Vector3 **out_size, const struct Vector3 **out_pos, const struct Vector4 **out_rot, const char **out_ame, const char **out_type) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Region_GetProperties(const struct Region *reg, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) LIBSWBF2_NOEXCEPT;

	// Scope //
	LIBSWBF2_API const struct Field **Scope_GetFields(struct Scope *, uint32_t hash, uint32_t *out_count) LIBSWBF2_NOEXCEPT;

	// Script //
	LIBSWBF2_API const char *Script_GetName(const struct Script *script) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Script_GetData(const struct Script *script, const uint8_t **out_data, uint32_t *out_size) LIBSWBF2_NOEXCEPT;

	// Segment //
	LIBSWBF2_API uint8_t Segment_FetchAllFields(const struct Segment *seg, uint8_t *out_pretx, const char **out_boneName, const char **out_tag, uint32_t *out_numVerts, Vector3 **out_pBuf, Vector3 **out_nBuf, Vector2 **out_uvBuf, uint32_t *out_numVWs, VertexWeight **out_vwBuf, int32_t *out_topo, uint32_t *out_numInds, uint16_t **out_iBuf, const Material **out_mat) LIBSWBF2_NOEXCEPT;

	// Sound //
	LIBSWBF2_API uint8_t Sound_GetData(const struct Sound *sound, uint32_t *out_sampleRate, uint32_t *out_sampleCount, uint8_t *out_blockAlign, const uint8_t **out_data) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Sound_FillDataBuffer(const struct Sound *sound, void *buffer) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t Sound_FetchAllFields(const struct Sound *soundPtr, uint32_t *out_format, int32_t *out_numChannels, int32_t *out_sampleRate, int32_t *out_numSamples, uint32_t *out_alias, uint8_t *out_hasData, uint32_t *out_name, uint32_t *out_numBytes) LIBSWBF2_NOEXCEPT;

	// SoundBank //
	LIBSWBF2_API uint8_t SoundBank_FetchAllFields(const struct SoundBank *str, uint32_t *out_name, uint8_t *out_hasData, uint32_t *out_format) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t SoundBank_GetSound(const struct SoundBank *str, uint32_t soundName, const struct Sound* *out_sound) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t SoundBank_GetSounds(const struct SoundBank *str, const struct Sound **out_sounds, uint32_t *out_numSounds, uint32_t *out_soundInc) LIBSWBF2_NOEXCEPT;

	// SoundStream //
	LIBSWBF2_API uint8_t SoundStream_FetchAllFields(const struct SoundStream *str, uint32_t *out_name, uint8_t *out_hasData, uint32_t *out_format, uint32_t *out_numChannels, uint32_t *out_numSubstreams, uint32_t *out_substreamInterleave) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t SoundStream_GetSound(const struct SoundStream *str, uint32_t soundName, const struct Sound **out_sound) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API uint8_t SoundStream_GetSounds(const struct SoundStream *str, const struct Sound **out_sounds, uint32_t *out_numSounds, uint32_t *out_soundInc) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API int32_t SoundStream_SampleReadMethod(struct SoundStream *str, void *sBuf, int32_t sBufLength, int32_t numToRead, enum ESoundFormat format, int32_t *out_numBytesRead, bool ReadSamples) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool SoundStream_SetFileReader(struct SoundStream *str, struct FileReader *readerPtr) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool SoundStream_SetStreamBuffer(struct SoundStream *str, void *bufferPtr, int32_t bufferLgenth) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API int32_t SoundStream_ReadBytesFromStream(struct SoundStream *str, int32_t NumBytes) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API int32_t SoundStream_GetNumSamplesInBytes(struct SoundStream *str, int32_t NumBytes) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool SoundStream_SetSegment(struct SoundStream *str, const char *name) LIBSWBF2_NOEXCEPT;

	// STRP //
	LIBSWBF2_API void STRP_CalcPolygons(struct STRP *strp) LIBSWBF2_NOEXCEPT;

	// Terrain //
	LIBSWBF2_API uint8_t Terrain_FetchSimpleFields(const struct Terrain *ter, int32_t *out_numTexes, const char ***out_texNames, float *out_heightUpper, float *out_heightLower, uint32_t *out_numVerts, struct Vector3 **out_vBuf, uint32_t *out_numNormals, struct Vector3 **out_nBuf, uint32_t *out_numUVs, struct Vector2 **out_uvBuf) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Terrain_GetHeightMap(const struct Terrain *ter, uint32_t *out_dim, uint32_t *out_dimScale, float **out_heightData) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Terrain_GetBlendMap(const struct Terrain *ter, uint32_t *out_width, uint32_t *out_numLayers, uint8_t **out_data) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void Terrain_GetIndexBuffer(const struct Terrain *terr, uint32_t **out_indicies, uint32_t *out_numInds) LIBSWBF2_NOEXCEPT;

	// Texture //
	LIBSWBF2_API uint8_t Texture_FetchAllFields(const struct Texture *tex, uint16_t *out_width, uint16_t *out_height, const uint8_t **out_buf, const char **out_name) LIBSWBF2_NOEXCEPT;

	// World //
	LIBSWBF2_API uint8_t World_FetchAllFields(const struct World *world, const char **out_name, const char **out_skyName, const struct Terrain **out_terrPtr) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API bool World_GetChildrenList(const struct World *world, uint8_t listID, void **out_listPtr, int32_t *out_listCount, int32_t *out_wrapperSize) LIBSWBF2_NOEXCEPT;

	// WorldAnimation //
	LIBSWBF2_API uint8_t WorldAnimation_FetchAllFields(const struct WorldAnimation *anim, uint8_t *out_loop, uint8_t *out_localT, const char **out_namePtr) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void WorldAnimation_GetAnimKeys(const struct WorldAnimation *anim, struct WorldAnimationKey **out_keyBuff, int32_t *out_numKeys, uint8_t IsRotation) LIBSWBF2_NOEXCEPT;

	// WorldAnimationGroup //
	LIBSWBF2_API uint8_t WorldAnimationGroup_FetchAllFields(const struct WorldAnimationGroup *group, uint8_t *out_bool0, uint8_t *out_bool1, const char **out_namePtr) LIBSWBF2_NOEXCEPT;
	LIBSWBF2_API void WorldAnimationGroup_GetAnimInstPairs(const struct WorldAnimationGroup *group, const char ***out_animNames, const char ***out_instNames, int32_t *out_numKeys) LIBSWBF2_NOEXCEPT;

	// WorldAnimationHierarchy //
	LIBSWBF2_API uint8_t WorldAnimationHierarchy_FetchAllFields(const struct WorldAnimationHierarchy *hier, const char  **out_rootPtr, const char ***out_childNames, int32_t *out_numChildren) LIBSWBF2_NOEXCEPT;

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
} // namespace LibSWBF2
#endif
