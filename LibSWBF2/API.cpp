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

		TList<T> l;
		if (v.size() > 0) {
			l.size = v.size();
			l.elements = new T[l.size];
		}
		if constexpr (std::is_trivially_copyable_v<T>) {
			std::memcpy(l.elements, v.data(), l.element_size * l.size);
		} else {
			for (size_t i = 0; i < l.size; ++ i) {
				l[i] = std::move(v[i]);
			}
		}
		return l;
	}

	template<typename T> TList<T> vector_to_tlist(const std::vector<T> &v)
	{
		static_assert(sizeof(TList<T>) == sizeof(CList));
		static_assert(std::is_standard_layout_v<TList<T>>);

		TList<T> l;
		if (v.size() > 0) {
			l.size = v.size();
			l.elements = new T[l.size];
		}
		if constexpr (std::is_trivially_copyable_v<T>) {
			std::memcpy(l.elements, v.data(), l.element_size * l.size);
		} else {
			for (size_t i = 0; i < l.size; ++ i) {
				l[i] = v[i];
			}
		}
		return l;
	}

	// API List //

	void CList_free(struct CList *l) noexcept
	{
		free(l->elements);
		free(l);
	}

	// Helpers //

	uint32_t FNVHashString(const char *string) noexcept
	{
		return catchall([&] {
			return FNV::Hash(string);
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

	TList<Instance> World_GetInstances(const World *world) noexcept
	{
		return catchall([&] {
			CHECKNULL(world, TList<Instance>{});
			return vector_to_tlist(world->GetInstances());
		});
	}

// XXX Everything below this line is a work in progress
#if 0
	// AnimationBank //

	bool AnimationBank_GetAnimationMetadata(const AnimationBank *setPtr, uint32_t animCRC, int32_t *out_numFrames, int32_t *out_numBones) noexcept
	{
		return catchall([&] {
			CheckPtr(setPtr, false);
			uint32_t frames, bones;
			bool status = setPtr->GetAnimationMetadata(animCRC, frames, bones);

			*out_numFrames = frames;
			*out_numBones = bones;
			return status;
		});
	}

	// Bone //

	const char *Bone_GetName(const Bone *bone) noexcept
	{
		return catchall([&] {
			CheckPtr(bone, static_cast<const char *>(nullptr));
			return bone->m_BoneName;
		});
	}

	const char *Bone_GetParentName(const Bone *bone) noexcept
	{
		return catchall([&] {
			CheckPtr(bone, static_cast<const char *>(nullptr));
			return bone->m_Parent;
		});
	}

	Vector3 Bone_GetPosition(const Bone *bone) noexcept
	{
		return catchall([&] {
			CheckPtr(bone, Vector3{});
			return bone->m_Position;
		});
	}

	Vector4 Bone_GetRotation(const Bone *bone) noexcept
	{
		return catchall([&] {
			CheckPtr(bone, Vector4{});
			return bone->m_Rotation;
		});
	}

	// CollisionMesh //

	void CollisionMesh_GetIndexBuffer(const CollisionMesh *mesh, uint16_t **out_indices, uint32_t *out_numInds) noexcept
	{
		catchall([&] {
			if (mesh == nullptr) {
				*out_numInds = 0;
			} else {
				mesh->GetIndexBuffer(ETopology::TriangleList, *out_numInds, *out_indices);
			}
		});
	}

	void CollisionMesh_GetVertexBuffer(const CollisionMesh *mesh, Vector3 **out_verts, uint32_t *out_numVerts) noexcept
	{
		catchall([&] {
			if (mesh == nullptr) {
				*out_numVerts = 0;
			} else {
				mesh->GetVertexBuffer(*out_numVerts, *out_verts);
			}
		});
	}

	// CollisionPrimitive //

	const char *CollisionPrimitive_GetParentName(const CollisionPrimitive *cp) noexcept
	{
		return catchall([&] {
			CheckPtr(cp, static_cast<const char *>(nullptr));
			return cp->GetParentName();
		});
	}

	Vector3 CollisionPrimitive_GetPosition(const CollisionPrimitive *cp) noexcept
	{
		return catchall([&] {
			CheckPtr(cp, Vector3{});
			return cp->GetPosition();
		});
	}

	Vector4 CollisionPrimitive_GetRotation(const CollisionPrimitive *cp) noexcept
	{
		return catchall([&] {
			CheckPtr(cp, Vector4{});
			return cp->GetRotation();
		});
	}

	ECollisionPrimitiveType CollisionPrimitive_GetType(const CollisionPrimitive *cp) noexcept
	{
		return catchall([&] {
			CheckPtr(cp, static_cast<ECollisionPrimitiveType>(0));
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

	const Field *Config_GetField(const Config *cfg, uint32_t hash) noexcept
	{
		return catchall([&] {
			CheckPtr(cfg, static_cast<const Field *>(nullptr));
			return &cfg->GetField(hash);
		});
	}

	const Field **Config_GetFields(const Config *cfg, uint32_t hash, size_t *out_fieldCount) noexcept
	{
		return catchall([&] {
			CheckPtr(cfg, (const Field **)nullptr);
			const std::vector<const Field *> fields = cfg->GetFields(hash);
			*out_fieldCount = fields.size();
			return fields.data();
		});
	}

	// Connection //

	const void *Connection_GetFieldPtr(const Connection *con, uint8_t id) noexcept
	{
		return catchall([&] {
			CheckPtr(con, (const void *)nullptr);
			switch (id)
			{
				case 0:
					return (const void *)con->m_Name;
				case 1:
					return (const void *)&(con->m_Start);
				case 2:
					return (const void *)&(con->m_End);
				case 3:
					return (const void *)&(con->m_FilterFlags);
				case 4:
					return (const void *)&(con->m_AttributeFlags);
				default:
					return (const void *)nullptr;
			}
		});
	}

	// Container //

	Container *Container_Initialize() noexcept
	{
		return catchall([&] {
			return Container::Create();
		});
	}

	const Level *Container_AddLevel(Container *container, const char *path) noexcept
	{
		return catchall([&] {
			CheckPtr(container, static_cast<const Level *>(nullptr))
			return const_cast<const Level *>(container->AddLevel(path));
		});
	}

	const Level *Container_GetLevel(Container *container, uint32_t handleNum) noexcept
	{
		return catchall([&] {
			CheckPtr(container,(Level*)nullptr);
			return container->GetLevel((uint16_t) handleNum);
		});
	}

	const Config *Container_FindConfig(Container *container, EConfigType type, uint32_t nameHash) noexcept
	{
		return catchall([&] {
			CheckPtr(container, (const Config *)nullptr);
			return container->FindConfig(type, nameHash);
		});
	}

	const Texture *Container_FindTexture(Container *container, uint32_t nameHash) noexcept
	{
		return catchall([&] {
			CheckPtr(container, (const Texture *)nullptr);
			return container->FindTexture(nameHash);
		});
	}

	const Model *Container_FindModel(Container *container, uint32_t nameHash) noexcept
	{
		return catchall([&] {
			CheckPtr(container, (const Model *)nullptr);
			return container->FindModel(nameHash);
		});
	}

	const EntityClass *Container_FindEntityClass(Container *container, uint32_t nameHash) noexcept
	{
		return catchall([&] {
			CheckPtr(container, (const EntityClass *)nullptr);
			return container->FindEntityClass(nameHash);
		});
	}

	bool Container_Delete(Container *container) noexcept
	{
		return catchall([&] {
			CheckPtr(container, false);
			Container::Delete(container);
			return true;
		});
	}

	// EntityClass //

	const char *EntityClass_GetName(const EntityClass *ec) noexcept
	{
		return catchall([&] {
			CheckPtr(ec, static_cast<const char *>(nullptr))
			return ec->GetTypeName();
		});
	}

	const EntityClass *EntityClass_GetBase(const EntityClass *ec) noexcept
	{
		return catchall([&] {
			CheckPtr(ec, (const EntityClass *)nullptr)
			return ec->GetBase();
		});
	}

	const char *EntityClass_GetBaseName(const EntityClass *ec) noexcept
	{
		return catchall([&] {
			CheckPtr(ec, static_cast<const char *>(nullptr))
			return ec->GetBaseName();
		});
	}

	// Field //

	uint32_t Field_GetNameHash(const Field *cfg) noexcept
	{
		return catchall([&] {
			return cfg->GetNameHash();
		});
	}

	uint8_t Field_GetNumValues(const Field *cfg) noexcept
	{
		return catchall([&] {
			return cfg->GetNumValues();
		});
	}

	float Field_GetFloat(const Field *cfg, uint8_t index) noexcept
	{
		return catchall([&] {
			return cfg->GetFloat(index);
		});
	}

	uint32_t Field_GetUInt32(const Field *cfg, uint8_t index) noexcept
	{
		return catchall([&] {
			return cfg->GetUInt32(index);
		});
	}

	const char *Field_GetString(const Field *cfg, uint8_t index) noexcept
	{
		return catchall([&] {
			CheckPtr(cfg, static_cast<const char *>(nullptr));
			return cfg->GetString(index);
		});
	}

	const char *Field_GetName(const Field *cfg) noexcept
	{
		return catchall([&] {
			CheckPtr(cfg, static_cast<const char *>(nullptr));
			return cfg->GetName();
		});
	}

	const Scope *Field_GetScope(const Field *field) noexcept
	{
		return catchall([&] {
			CheckPtr(field, static_cast<const Scope *>(nullptr));
			return &field->m_Scope;
		});
	}

	// Hub //

	const void *Hub_GetFieldPtr(const Hub *hub, uint8_t id, int32_t *out_numBytes) noexcept
	{
		return catchall([&] {
			CheckPtr(hub, (const void *)nullptr);
			switch (id)
			{
				case 0:
					return (const void *)hub->m_Name;
				case 1:
					return (const void *)&(hub->m_Position);
				case 2:
					return (const void *)&(hub->m_Radius);
				case 3:
					return (const void *)hub->m_ConnectionIndices;
				case 4:
					return (const void *)hub->m_ConnectionsPerLayer;
				case 5:
				{
					*out_numBytes = (int32_t)hub->m_QuantizedDataBuffer.size();
					return (const void *)hub->m_QuantizedDataBuffer.data();
				}
				default:
					return (const void *)nullptr;
			}
		});
	}

	// Instance //

	const char *Instance_GetName(const Instance *instance) noexcept
	{
		return catchall([&] {
			CheckPtr(instance, static_cast<const char *>(nullptr));
			return instance->GetName();
		});
	}

	const char *Instance_GetEntityClassName(const Instance *instance) noexcept
	{
		return catchall([&] {
			CheckPtr(instance, static_cast<const char *>(nullptr));
			return instance->GetEntityClassName();
		});
	}

	Vector3 Instance_GetPosition(const Instance *instance) noexcept
	{
		return catchall([&] {
			CheckPtr(instance, Vector3{});
			return instance->GetPosition();
		});
	}

	Vector4 Instance_GetRotation(const Instance *instance) noexcept
	{
		return catchall([&] {
			CheckPtr(instance, Vector4{});
			return instance->GetRotation();
		});
	}

	// Level //

	uint8_t Level_IsWorldLevel(const Level *level) noexcept
	{
		return catchall([&] {
			CheckPtr(level, static_cast<uint8_t>(0));
			return (uint8_t)level->IsWorldLevel();
		});
	}

	const char *Level_GetName(const Level *level) noexcept
	{
		return catchall([&] {
			CheckPtr(level, static_cast<const char *>(nullptr));
			return level->GetLevelName();
		});
	}

	const Config *Level_GetConfig(const Level *level, uint32_t header, uint32_t hash) noexcept
	{
		return catchall([&] {
			const Config *ptr = nullptr;
			EConfigType cfgType = static_cast<EConfigType>(header);

			ptr = level->GetConfig(cfgType, hash);

			return ptr;
		});
	}

	SoundStream *Level_FindAndIndexSoundStream(Level *level, FileReader *stream, uint32_t StreamName) noexcept
	{
		return catchall([&] {
			CheckPtr(stream, (SoundStream *)nullptr);
			CheckPtr(level, (SoundStream *)nullptr);

			return level->FindAndIndexSoundStream(*stream, StreamName);
		});
	}

	size_t Level_GetWorldCount(const Level *level) noexcept
	{
		return catchall([&] {
			CheckPtr(level, static_cast<size_t>(0));
			return level->GetWorlds().size();
		});
	}

	const World *Level_GetWorld(const Level *level, size_t index) noexcept
	{
		return catchall([&] {
			CheckPtr(level, static_cast<const World *>(nullptr));
			return &level->GetWorlds()[index];
		});
	}

	// Localization //

	const char *Localization_GetName(const Localization *local) noexcept
	{
		return catchall([&] {
			CheckPtr(local, static_cast<const char *>(nullptr));
			return local->GetName();
		});
	}

	uint8_t Localization_GetLocalizedWideString(const Localization *local, const char *path, uint16_t **out_chars, uint32_t *out_count) noexcept
	{
		return catchall([&] {
			CheckPtr(local, static_cast<uint8_t>(0));
			return (uint8_t)local->GetLocalizedWideString(path, *out_chars, *out_count);
		});
	}

	// Logging //

	void LOG_SetLogfileLevel(ELogType LogfileLevel) noexcept
	{
		catchall([&] {
			Logging::Logger::SetLogfileLevel(LogfileLevel);
		});
	}

	// Material //

	const Texture *Material_GetTexture(const Material *mat, size_t index) noexcept
	{
		return catchall([&] {
			CheckPtr(mat, (const Texture *)nullptr);
			return mat->GetTexture(index);
		});
	}

	EMaterialFlags Material_GetFlags(const Material *mat) noexcept
	{
		return catchall([&] {
			CheckPtr(mat, (EMaterialFlags)0);
			return mat->GetFlags();
		});
	}

	// Memory //

	void Memory_Blit(void *dest, void *src, int numBytes) noexcept
	{
		catchall([&] {
			memcpy(dest, src, numBytes);
		});
	}

	// Model //

	size_t Model_GetSegmentCount(const Model *model) noexcept
	{
		return catchall([&] {
			CheckPtr(model, static_cast<size_t>(0));
			return model->GetSegments().size();
		});
	}

	const Segment *Model_GetSegment(const Model *model, size_t index) noexcept
	{
		return catchall([&] {
			CheckPtr(model, static_cast<const Segment *>(nullptr));
			return &model->GetSegments()[index];
		});
	}

	size_t Model_GetBoneCount(const Model *model) noexcept
	{
		return catchall([&] {
			CheckPtr(model, static_cast<size_t>(0));
			return model->GetBones().size();
		});
	}

	const Bone *Model_GetBone(const Model *model, size_t index) noexcept
	{
		return catchall([&] {
			CheckPtr(model, static_cast<const Bone *>(nullptr));
			return &model->GetBones()[index];
		});
	}

	const CollisionMesh *Model_GetCollisionMesh(const Model *model) noexcept
	{
		return catchall([&] {
			CheckPtr(model, static_cast<const CollisionMesh *>(nullptr));
			return &model->GetCollisionMesh();
		});
	}

	size_t Model_GetCollisionPrimitiveCount(const Model *model) noexcept
	{
		return catchall([&] {
			CheckPtr(model, static_cast<size_t>(0));
			return model->GetCollisionPrimitives().size();
		});
	}

	const CollisionPrimitive *Model_GetCollisionPrimitive(const Model *model, size_t index) noexcept
	{
		return catchall([&] {
			CheckPtr(model, static_cast<const CollisionPrimitive *>(nullptr));
			return const_cast<const CollisionPrimitive *>(&model->GetCollisionPrimitives()[index]);
		});
	}

	// Scope //

	const Field *Scope_GetField(const Scope *scope, uint32_t hash) noexcept
	{
		return catchall([&] {
			CheckPtr(scope, static_cast<const Field *>(nullptr));
			return &scope->GetField(hash);
		});
	}

	const Field **Scope_GetFields(const Scope *scope, uint32_t hash, size_t *out_fieldCount) noexcept
	{
		return catchall([&] {
			CheckPtr(scope, (const Field **)nullptr);
			const std::vector<const Field *> fields = scope->GetFields(hash);
			*out_fieldCount = fields.size();
			return fields.data();
		});
	}

	// Segment //

	void Segment_GetIndexBuffer(const Segment *seg, uint16_t **out_indices, uint32_t *out_numInds) noexcept
	{
		catchall([&] {
			if (seg == nullptr) {
				*out_numInds = 0;
			} else {
				seg->GetIndexBuffer(*out_numInds, *out_indices);
			}
		});
	}

	void Segment_GetVertexBuffer(const Segment *seg, Vector3 **out_verts, uint32_t *out_numVerts) noexcept
	{
		catchall([&] {
			if (seg == nullptr) {
				*out_numVerts = 0;
			} else {
				seg->GetVertexBuffer(*out_numVerts, *out_verts);
			}
		});
	}

	void Segment_GetUVBuffer(const Segment *seg, Vector2 **out_uvs, uint32_t *out_numUVs) noexcept
	{
		catchall([&] {
			if (seg == nullptr) {
				*out_numUVs = 0;
			} else {
				seg->GetUVBuffer(*out_numUVs, *out_uvs);
			}
		});
	}

	void Segment_GetNormalBuffer(const Segment *seg, Vector3 **out_normals, uint32_t *out_numNormals) noexcept
	{
		catchall([&] {
			if (seg == nullptr) {
				*out_numNormals = 0;
			} else {
				seg->GetNormalBuffer(*out_numNormals, *out_normals);
			}
		});
	}

	ETopology Segment_GetTopology(const Segment *seg) noexcept
	{
		return catchall([&] {
			CheckPtr(seg, static_cast<ETopology>(0));
			return seg->GetTopology();
		});
	}

	const Material *Segment_GetMaterial(const Segment *seg) noexcept
	{
		return catchall([&] {
			CheckPtr(seg, (const Material *)nullptr);
			return &seg->GetMaterial(); // Segment returns a reference to Material... Why?
		});
	}

	const char *Segment_GetBoneName(const Segment *seg) noexcept
	{
		return catchall([&] {
			CheckPtr(seg, static_cast<const char *>(nullptr));
			return seg->GetBone();
		});
	}

	// Sound //

	uint8_t Sound_GetData(const Sound *sound, uint32_t *out_sampleRate, uint32_t *out_sampleCount, uint8_t *out_blockAlign, const uint8_t **out_data) noexcept
	{
		return catchall([&] {
			CheckPtr(sound, static_cast<uint8_t>(0));
			return (uint8_t)sound->GetData(*out_sampleRate, *out_sampleCount, *out_blockAlign, *out_data);
		});
	}

	uint8_t Sound_FillDataBuffer(const Sound *sound, void *buffer) noexcept
	{
		return catchall([&] {
			CheckPtr(sound, false);
			return sound->FillDataBuffer(ESoundFormat::PCM16, (int16_t *)buffer);
		});
	}

	// SoundBank //

	uint8_t SoundBank_GetSound(const SoundBank *bnk, uint32_t soundName, const Sound **out_sound) noexcept
	{
		return catchall([&] {
			CheckPtr(bnk, false);
			*out_sound = bnk->GetSound(soundName);
			return *out_sound != nullptr;
		});
	}

	uint8_t SoundBank_GetSounds(const SoundBank *bnk, const Sound **out_sounds, uint32_t *out_numSounds, uint32_t *out_soundInc) noexcept
	{
		return catchall([&] {
			CheckPtr(bnk, false);
			const std::vector<Sound>& sounds = bnk->GetSounds();
			*out_sounds = sounds.data();
			*out_numSounds = sounds.size();
			*out_soundInc = sizeof(Sound);
			return *out_numSounds > 0;
		});
	}

	// SoundStream //

	uint8_t SoundStream_GetSound(const SoundStream *str, uint32_t soundName, const Sound **out_sound) noexcept
	{
		return catchall([&] {
			CheckPtr(str, false);
			*out_sound = str->GetSound(soundName);
			return *out_sound != nullptr;
		});
	}

	uint8_t SoundStream_GetSounds(const SoundStream *str, const Sound **out_sounds, uint32_t *out_numSounds, uint32_t *out_soundInc) noexcept
	{
		return catchall([&] {
			CheckPtr(str, false);
			const std::vector<Sound>& sounds = str->GetSounds();
			*out_sounds = sounds.data();
			*out_numSounds = sounds.size();
			*out_soundInc = sizeof(Sound);
			return *out_numSounds > 0;
		});
	}

	int32_t SoundStream_SampleReadMethod(SoundStream *str, void *sBuf, int32_t sBufLength, int32_t numToRead, ESoundFormat format, int32_t *out_numBytesRead, bool ReadSamples) noexcept
	{
		return catchall([&] {
			CheckPtr(str, -1);
			if (ReadSamples) {
				return str->ReadSamples(sBuf, sBufLength, numToRead, format);
			} else {
				// This method might not be needed, it depends on whether or not we
				// encounter other formats or if IMAADPCM block boundaries can be violated...
				return -1;// str->ReadSamplesFromBytes(sBuf, sBufLength, numToRead, format, *out_numBytesRead);
			}
		});
	}

	int32_t SoundStream_GetNumSamplesInBytes(SoundStream *str, int32_t NumBytes) noexcept
	{
		return catchall([&] {
			CheckPtr(str, -1);
			return str->GetNumSamplesInBytes(NumBytes);
		});
	}

	bool SoundStream_SetFileReader(SoundStream *str, FileReader *readerPtr) noexcept
	{
		return catchall([&] {
			CheckPtr(str, false);
			return str->SetFileReader(readerPtr);
		});
	}

	bool SoundStream_SetStreamBuffer(SoundStream *str, void *bufferPtr, int32_t bufferLgenth) noexcept
	{
		return catchall([&] {
			CheckPtr(str, false);
			return str->SetFileStreamBuffer((uint8_t *)bufferPtr, bufferLgenth);
		});
	}

	int32_t SoundStream_ReadBytesFromStream(SoundStream *str, int32_t numBytes) noexcept
	{
		return catchall([&] {
			CheckPtr(str, -1);
			return str->ReadBytesFromStream(numBytes);
		});
	}

	bool SoundStream_SetSegment(SoundStream *str, const char *name) noexcept
	{
		return catchall([&] {
			CheckPtr(str, false);
			return str->SetSegment(FNV::Hash(name));
		});
	}

	// STRP //

	void STRP_CalcPolygons(Chunks::MSH::STRP *strp) noexcept
	{
		catchall([&] {
			CheckPtr(strp)
			strp->CalcPolygons();
		});
	}

	// Terrain //

	void Terrain_GetHeightMap(const Terrain *ter, uint32_t *out_dim, uint32_t *out_dimScale, float **out_heightData) noexcept
	{
		catchall([&] {
			*out_dim = 0;
			CheckPtr(ter);
			ter->GetHeightMap(*out_dim, *out_dimScale, *out_heightData);
		});
	}

	void Terrain_GetBlendMap(const Terrain *ter, uint32_t *out_dim, uint32_t *out_numLayers, uint8_t **out_data) noexcept
	{
		catchall([&] {
			*out_dim = 0;
			CheckPtr(ter);
			ter->GetBlendMap(*out_dim, *out_numLayers, *out_data);
		});
	}

	void Terrain_GetIndexBuffer(const Terrain *terr, uint32_t **out_indices, uint32_t *out_numInds) noexcept
	{
		catchall([&] {
			if (terr == nullptr || !terr->GetIndexBuffer(ETopology::TriangleList, *out_numInds, *out_indices)) {
				*out_numInds = 0;
			}
		});
	}

	void Terrain_GetVertexBuffer(const Terrain *terr, Vector3 **out_verts, uint32_t *out_numVerts) noexcept
	{
		catchall([&] {
			if (terr == nullptr) {
				*out_numVerts = 0;
			} else {
				terr->GetVertexBuffer(*out_numVerts, *out_verts);
			}
		});
	}

	void Terrain_GetUVBuffer(const Terrain *terr, Vector2 **out_uvs, uint32_t *out_numUVs) noexcept
	{
		catchall([&] {
			if (terr == nullptr) {
				*out_numUVs = 0;
			} else {
				terr->GetUVBuffer(*out_numUVs, *out_uvs);
			}
		});
	}

	size_t Terrain_GetLayerTextureCount(const Terrain *terrain) noexcept
	{
		return catchall([&] {
			CheckPtr(terrain, (size_t)0);
			return terrain->GetLayerTextures().size();
		});
	}

	const char *Terrain_GetLayerTextureName(const Terrain *terrain, size_t index) noexcept
	{
		return catchall([&] {
			CheckPtr(terrain, static_cast<const char *>(nullptr));
			return terrain->GetLayerTextures()[index];
		});
	}

	// World //

	const char *World_GetSkyName(const World *world) noexcept
	{
		return catchall([&] {
			CheckPtr(world, static_cast<const char *>(nullptr));
			return world->GetSkyName();
		});
	}

	const Terrain *World_GetTerrain(const World *world) noexcept
	{
		return catchall([&] {
			CheckPtr(world, (const Terrain *)nullptr);
			return world->GetTerrain();
		});
	}

	const char *World_GetTerrainName(const World *world) noexcept
	{
		return catchall([&] {
			CheckPtr(world, static_cast<const char *>(nullptr));
			return world->GetTerrainName();
		});
	}
#endif
}
