#include "pch.h"
#include "API.h"
#include "InternalHelpers.h"
#include "Types/LibString.h"
#include "Types/Enums.h"
#include "Types/Planning.h"
#include "Chunks/MSH/MSH.h"

#include <string.h>
#include "Container.h"
#include "Wrappers/Wrappers.h"

#include "Chunks/HeaderNames.h"

#include "MemoryMappedReader.h"
#include "StreamReader.h"

namespace LibSWBF2
{
#define CheckPtr(PTR, ...) if (PTR == nullptr) { LOG_ERROR("[API] Given Pointer was NULL!"); return __VA_ARGS__; }

	template<typename T> auto catchall(T &&t) noexcept->decltype(t())
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

	static void GetStringListPtrs(const List<String> &strings, List<const char*> &outPtrs)
	{
		outPtrs.Clear();
		for (int i = 0; i < strings.Size(); i++) {
			outPtrs.Add(strings[i].Buffer());
		}
	}

	template<class T1, class T2>
	uint8_t GenericGetProperty(const T1 *ptr, T2 prop, const char *&value)
	{
		CheckPtr(ptr, false);
		static String propValue;
		if (ptr->GetProperty(prop, propValue))
		{
			value = propValue.Buffer();
			return true;
		}
		value = nullptr;
		return false;
	}

	template<class T1, class T2>
	uint8_t GenericGetProperties(const T1 *ptr, T2 prop, const char **&values, uint32_t &count)
	{
		CheckPtr(ptr, false);
		static List<String> propValues;
		static List<const char*> propValuePtrs;
		propValues.Clear();
		propValuePtrs.Clear();
		if (ptr->GetProperty(prop, propValues))
		{
			GetStringListPtrs(propValues, propValuePtrs);
			values = propValuePtrs.GetArrayPtr();
			count = (uint32_t)propValuePtrs.Size();
			return true;
		}
		values = nullptr;
		count = 0;
		return false;
	}

	// AnimationBank //

	bool AnimationBank_GetCurve(const AnimationBank *setPtr, uint32_t animCRC, uint32_t boneCRC, uint32_t comp, const uint16_t **out_indicesBuffer, const float **out_valuesBuffer, int *out_numKeys) noexcept
	{
		return catchall([&] {
			static List<uint16_t> indices;
			static List<float> values;

			CheckPtr(setPtr, false);

			bool status = setPtr->GetCurve(animCRC, boneCRC, comp, indices, values);

			if (status)
			{
				*out_numKeys = (int32_t)values.Size();
				*out_indicesBuffer = indices.GetArrayPtr();
				*out_valuesBuffer = values.GetArrayPtr();
			}

			return status;
		});
	}

	const uint32_t *AnimationBank_GetAnimationCRCs(const AnimationBank *setPtr, int *out_numCRCs) noexcept
	{
		return catchall([&] {
			static List<uint32_t> crcs;

			*out_numCRCs = 0;
			CheckPtr(setPtr, (uint32_t *)nullptr);

			crcs = setPtr->GetAnimations();

			*out_numCRCs = (int32_t)crcs.Size();
			return crcs.GetArrayPtr();
		});
	}

	const uint32_t *AnimationBank_GetBoneCRCs(const AnimationBank *setPtr, uint32_t animCRC, int *out_numCRCs) noexcept
	{
		return catchall([&] {
			static List<uint32_t> crcs;

			*out_numCRCs = 0;
			CheckPtr(setPtr, (uint32_t *)nullptr);

			crcs = setPtr->GetBones(animCRC);

			*out_numCRCs = (int32_t)crcs.Size();
			return crcs.GetArrayPtr();
		});
	}

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

	// AnimationSkeleton //

	const char *AnimationSkeleton_GetName(const AnimationSkeleton *skelPtr) noexcept
	{
		return catchall([&] {
			static String nameCache;
			CheckPtr(skelPtr, (const char *)nullptr);
			nameCache = skelPtr->GetName();

			return nameCache.Buffer();
		});
	}

	uint8_t AnimationSkeleton_GetJoints(const AnimationSkeleton *skelPtr, int32_t *out_numJoints, Joint **out_jointsPtr) noexcept
	{
		return catchall([&] {
			static List<Joint> jointsCache;
			CheckPtr(skelPtr, false);

			if (skelPtr->GetJoints(jointsCache))
			{
				*out_numJoints = (int32_t) jointsCache.Size();
				*out_jointsPtr = jointsCache.GetArrayPtr();
				return true;
			}
			else
			{
				*out_numJoints = 0;
				return false;
			}
		});
	}

	// Barrier //

	const void *Barrier_GetFieldPtr(const Barrier *bar, uint8_t fieldID) noexcept
	{
		return catchall([&] {
			CheckPtr(bar, (const void *)nullptr);
			static Vector4 rotCache;

			const void *fieldPtr = nullptr;
			switch (fieldID)
			{
				case 0:
					fieldPtr = (void *)&(bar->GetPosition());
					break;
				case 1:
					rotCache = bar->GetRotation();
					fieldPtr = (void *)&rotCache;
					break;
				case 2:
					fieldPtr = (void *)&(bar->GetFlag());
					break;
				case 3:
					fieldPtr = (void *)bar->GetName().Buffer();
					break;
				case 4:
					fieldPtr = (void *)&(bar->GetSize());
					break;
				default:
					break;
			}

			return fieldPtr;
		});
	}

	// Bone //

	void Bone_FetchAllFields(const Bone *bone, const char **out_name, const char **out_parentName, const Vector3 **out_loc, const Vector4 **out_rot) noexcept
	{
		catchall([&] {
			*out_name = bone->m_BoneName.Buffer();
			*out_loc = &(bone->m_Position);
			*out_rot = &(bone->m_Rotation);
			*out_parentName = bone->m_Parent.Buffer();
		});
	}

	// CollisionMesh //

	uint8_t CollisionMesh_FetchAllFields(const CollisionMesh *cmPtr, uint32_t *out_iCount, uint16_t **out_iBuf, uint32_t *out_vCount, Vector3 **out_vBuf, uint32_t *out_maskFlags, const char **out_namePtr, const char **out_nodeNamePtr) noexcept
	{
		return catchall([&] {
			CheckPtr(cmPtr, (uint8_t)0);

			static String name;
			static String nodeName;

			cmPtr->GetIndexBuffer(ETopology::TriangleList, *out_iCount, *out_iBuf);
			cmPtr->GetVertexBuffer(*out_vCount, *out_vBuf);
			*out_maskFlags = (uint32_t) cmPtr->GetMaskFlags();

			name = cmPtr->GetName();
			*out_namePtr = name.Buffer();

			nodeName = cmPtr->GetNodeName();
			*out_nodeNamePtr = nodeName.Buffer();

			return (uint8_t)1;
		});
	}


	// CollisionPrimitive //

	void CollisionPrimitive_FetchAllFields(CollisionPrimitive *primPtr, float *out_f1, float *out_f2, float *out_f3, const char **out_namePtr, const char **out_parentNamePtr, uint32_t *out_maskFlags, uint32_t *out_primitiveType, Vector3 **out_pos, Vector4 **out_rot) noexcept
	{
		catchall([&] {
			static String name, parentName;
			static Vector3 posTemp;
			static Vector4 rotTemp;

			*out_f1 = *out_f2 = *out_f3 = 0.0f;

			name = primPtr->GetName();
			parentName = primPtr->GetParentName();

			*out_namePtr = name.Buffer();
			*out_parentNamePtr = parentName.Buffer();

			*out_maskFlags = (uint32_t) primPtr->GetMaskFlags();
			*out_primitiveType = (uint32_t) primPtr->GetPrimitiveType();

			rotTemp = primPtr->GetRotation();
			posTemp = primPtr->GetPosition();

			*out_pos = &posTemp;
			*out_rot = &rotTemp;

			switch (*out_primitiveType) {
				case 1:
					primPtr->GetSphereRadius(*out_f1);
					return;
				case 2:
					primPtr->GetCylinderDims(*out_f1, *out_f2);
					return;
				case 4:
					primPtr->GetCubeDims(*out_f1, *out_f2, *out_f3);
					return;
				default:
					return;
			}
		});
	}

	// Config //

	uint8_t Config_FetchSimpleFields(const Config *cfg, uint32_t *out_name) noexcept
	{
		return catchall([&] {
			CheckPtr(cfg, false);
			*out_name = cfg->m_Name;
			return true;
		});
	}

	const Field **Config_GetFields(Config *ptr, uint32_t hash, uint32_t *out_count) noexcept
	{
		return catchall([&] {
			static List<const Field *> cache;
			cache = ptr->GetFields(hash);

			*out_count = (uint32_t)cache.Size();
			return cache.GetArrayPtr();
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
					return (const void *)con->m_Name.Buffer();
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

	const Container *Container_Initialize() noexcept
	{
		return catchall([&] {
			return Container::Create();
		});
	}

	uint16_t Container_AddLevel(Container *container, const char *path) noexcept
	{
		return catchall([&] {
			CheckPtr(container, UINT32_MAX)
			return (uint32_t) container->AddLevel(path);
		});
	}

	uint16_t Container_AddLevelFiltered(Container *container, const char *path, const char **subLVLs, uint32_t subLVLCount) noexcept
	{
		return catchall([&] {
			CheckPtr(container, UINT32_MAX)
			CheckPtr(subLVLs, UINT32_MAX)

			List<String> filter;
			for (uint32_t i = 0; i < subLVLCount; ++i)
			{
				filter.Add(subLVLs[i]);
			}

			return (uint32_t)container->AddLevel(path, &filter);
		});
	}

	void Container_FreeAll(Container *container, uint8_t force) noexcept
	{
		catchall([&] {
			CheckPtr(container)
			container->FreeAll((bool)force);
		});
	}

	float Container_GetProgress(Container *container, uint32_t handleNum) noexcept
	{
		return catchall([&] {
			CheckPtr(container, -1.0f)
			return container->GetLevelProgress((uint16_t) handleNum);
		});
	}

	float Container_GetOverallProgress(Container *container) noexcept
	{
		return catchall([&] {
			CheckPtr(container, -1.0f)
			return container->GetOverallProgress();
		});
	}

	bool Container_IsDone(Container *container) noexcept
	{
		return catchall([&] {
			CheckPtr(container, false);
			return container->IsDone();
		});
	}

	const Level *Container_GetLevel(Container *container, uint32_t handleNum) noexcept
	{
		return catchall([&] {
			CheckPtr(container,(Level*)nullptr);
			return container->GetLevel((uint16_t) handleNum);
		});
	}

	void Container_GetLoadedLevels(Container *container, uint16_t **out_handles, uint16_t handleCount) noexcept
	{
		catchall([&] {
			CheckPtr(container);
			static List<uint16_t> HANDLES;
			HANDLES = container->GetLoadedLevels();
			*out_handles = HANDLES.GetArrayPtr();
			handleCount = (uint16_t)HANDLES.Size();
		});
	}

	uint8_t Container_GetStatus(Container *container, uint32_t handle) noexcept
	{
		return catchall([&] {
			CheckPtr(container, (uint8_t)ELoadStatus::Uninitialized);
			return (uint8_t)container->GetStatus(handle);
		});
	}

	void Container_LoadLevels(Container *container) noexcept
	{
		catchall([&] {
			CheckPtr(container);
			container->StartLoading();
		});
	}

	const void *Container_GetWrapperFNV(Container *container, uint32_t type, uint32_t name) noexcept
	{
		return catchall([&] {
			switch (type)
			{
				case 1:
					return static_cast<const void *>(container->FindModel(name));
				case 2:
					return static_cast<const void *>(container->FindTexture(name));
				case 3:
					return static_cast<const void *>(container->FindWorld(name));
				case 4:
					return static_cast<const void *>(container->FindEntityClass(name));
				case 5:
					return static_cast<const void *>(container->FindAnimationBank(name));
				case 6:
					return static_cast<const void*>(container-> FindScript(name));
				case 7:
					return static_cast<const void*>(container-> FindSound(name));
				default:
					return (const void *)nullptr;
			}
		});
	}

	const void *Container_GetWrapper(Container *container, uint32_t type, const char *name) noexcept
	{
		return catchall([&] {
			return Container_GetWrapperFNV(container, type, (uint32_t) FNV::Hash(name));
		});
	}

	const Config *Container_GetConfig(Container *container, uint32_t type, uint32_t nameHash) noexcept
	{
		return catchall([&] {
			CheckPtr(container, (const Config *)nullptr);
			return container->FindConfig((EConfigType) type, nameHash);
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

	void EntityClass_FetchAllFields(const EntityClass *ec, const char **out_name, uint8_t *out_classType, const EntityClass **out_baseClass, const char **out_baseClassName) noexcept
	{
		catchall([&] {
			CheckPtr(ec);
			*out_name = ec->GetTypeName().Buffer();
			*out_classType = (uint8_t)ec->GetClassType();
			*out_baseClass = ec->GetBase();
			*out_baseClassName = ec->GetBaseName().Buffer();
		});
	}

	uint8_t EntityClass_GetPropertyFromName(const EntityClass *ec, const char *propName, const char **out_value) noexcept
	{
		return catchall([&] {
			return GenericGetProperty(ec, propName, *out_value);
		});
	}

	uint8_t EntityClass_GetPropertyFromHash(const EntityClass *ec, uint32_t hashedPropName, const char **out_value) noexcept
	{
		return catchall([&] {
			return GenericGetProperty(ec, hashedPropName, *out_value);
		});
	}

	uint8_t EntityClass_GetPropertiesFromName(const EntityClass *ec, const char *propName, const char ***out_values, uint32_t *out_count) noexcept
	{
		return catchall([&] {
			return GenericGetProperties(ec, propName, *out_values, *out_count);
		});
	}

	uint8_t EntityClass_GetPropertiesFromHash(const EntityClass *ec, uint32_t hashedPropName, const char ***out_values, uint32_t *out_count) noexcept
	{
		return catchall([&] {
			return GenericGetProperties(ec, hashedPropName, *out_values, *out_count);
		});
	}

	const char *EntityClass_GetName(const EntityClass *ec) noexcept
	{
		return catchall([&] {
			CheckPtr(ec, "")
			static String typeName;

			typeName = ec->GetTypeName();
			return typeName.Buffer();
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
			CheckPtr(ec, "")
			static String baseName;

			baseName = ec->GetBaseName();
			return baseName.Buffer();
		});
	}

	void EntityClass_GetOverriddenProperties(const EntityClass *ec, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) noexcept
	{
		return catchall([&] {
			CheckPtr(ec)
			static List<const char*> ptrsBuffer;
			static List<String> values;
			static List<uint32_t> hashes;

			ec->GetOverriddenProperties(hashes, values);

			*out_hashesBuffer = hashes.GetArrayPtr();
			*out_count = (int32_t)values.Size();
			GetStringListPtrs(values, ptrsBuffer);
			*out_valuesBuffer = ptrsBuffer.GetArrayPtr();
		});
	}

	void EntityClass_GetAllProperties(const EntityClass *ec, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) noexcept
	{
		return catchall([&] {
			CheckPtr(ec);
			static List<const char*> ptrsBuffer;
			static List<String> values;
			static List<uint32_t> hashes;

			ec->GetAllProperties(hashes, values);

			*out_hashesBuffer = hashes.GetArrayPtr();
			*out_count = (int32_t)values.Size();
			GetStringListPtrs(values, ptrsBuffer);
			*out_valuesBuffer = ptrsBuffer.GetArrayPtr();
		});
	}

	// Enum //

	const char *ENUM_TopologyToString(ETopology topology) noexcept
	{
		return catchall([&] {
			static Types::String lastToString;
			lastToString = TopologyToString(topology);
			return lastToString.Buffer();
		});
	}

	const char *ENUM_MaterialFlagsToString(EMaterialFlags flags) noexcept
	{
		return catchall([&] {
			static Types::String lastToString;
			lastToString = MaterialFlagsToString(flags);
			return lastToString.Buffer();
		});
	}

	const char *ENUM_VBUFFlagsToString(EVBUFFlags flags) noexcept
	{
		return catchall([&] {
			static String lastToString;
			lastToString = VBUFFlagsToString(flags);
			return lastToString.Buffer();
		});
	}

	// Field //

	uint8_t Field_FetchAllFields(const Field *cfg, Scope **out_scope) noexcept
	{
		return catchall([&] {
			CheckPtr(cfg, false);
			*out_scope = const_cast<Scope *>(&(cfg->m_Scope));
			return true;
		});
	}

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

	const Vector2 *Field_GetVec2(const Field *cfg) noexcept
	{
		return catchall([&] {
			static Vector2 cache;
			cache = cfg->GetVector2();
			return &cache;
		});
	}

	const Vector3 *Field_GetVec3(const Field *cfg) noexcept
	{
		return catchall([&] {
			static Vector3 cache;
			cache = cfg->GetVector3();
			return &cache;
		});
	}

	const Vector4 *Field_GetVec4(const Field *cfg) noexcept
	{
		return catchall([&] {
			static Vector4 cache;
			cache = cfg->GetVector4();
			return &cache;
		});
	}

	const char *Field_GetString(const Field *cfg, uint8_t index) noexcept
	{
		return catchall([&] {
			static String cache;
			cache = cfg->GetString(index);
			return cache.Buffer();
		});
	}

	const char *Field_GetName(const Field *cfg) noexcept
	{
		return catchall([&] {
			static String cache;
			cache = cfg->GetName();
			return cache.Buffer();
		});
	}

	// FileReader //

	const FileReader *FileReader_FromFile(const char *path, bool UseMemoryMapping) noexcept
	{
		return catchall([&] {
			FileReader *reader;
			if (UseMemoryMapping)
			{
				reader = new MemoryMappedReader();
			}
			else
			{
				reader = new StreamReader();
			}

			if (reader->Open(path))
			{
				return reader;
			}
			else
			{
				delete reader;
				return reader = nullptr;
			}
		});
	}

	void FileReader_Delete(FileReader *readerPtr) noexcept
	{
		catchall([&] {
			delete readerPtr;
		});
	}

	// Hash Lookup //

	uint8_t Hashing_Lookup(uint32_t hash, const char **out_str) noexcept
	{
		return catchall([&] {
			static String lookupCache;
			bool r = FNV::Lookup(hash, lookupCache);
			*out_str = lookupCache.Buffer();
			return r;
		});
	}

	// HintNode //

	const void *HintNode_GetFieldPtr(const HintNode *hnt, uint8_t fieldID) noexcept
	{
		return catchall([&] {
			CheckPtr(hnt, (const void *)nullptr);
			static Vector4 rotCache;

			const void *fieldPtr = nullptr;
			switch (fieldID)
			{
				case 0:
					fieldPtr = (void *)&(hnt->GetPosition());
					break;
				case 1:
					rotCache = hnt->GetRotation();
					fieldPtr = (void *)&rotCache;
					break;
				case 2:
					fieldPtr = (void *)&(hnt->GetType());
					break;
				case 3:
					fieldPtr = (void *)hnt->GetName().Buffer();
					break;
				default:
					break;
			}

			return fieldPtr;
		});
	}

	void HintNode_GetProperties(const HintNode *hnt, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) noexcept
	{
		return catchall([&] {
			CheckPtr(hnt);
			*out_count = 0;
			static List<const char*> ptrsBuffer;
			static List<String> values;
			static List<uint32_t> hashes;

			hnt->GetProperties(hashes, values);

			*out_hashesBuffer = hashes.GetArrayPtr();
			*out_count = (int32_t)values.Size();
			GetStringListPtrs(values, ptrsBuffer);
			*out_valuesBuffer = ptrsBuffer.GetArrayPtr();
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
					return (const void *)hub->m_Name.Buffer();
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
					*out_numBytes = (int32_t)hub->m_QuantizedDataBuffer.Size();
					return (const void *)hub->m_QuantizedDataBuffer.GetArrayPtr();
				}
				default:
					return (const void *)nullptr;
			}
		});
	}

	// Instance //

	uint8_t Instance_FetchSimpleFields(const Instance *instPtr, const char **out_name, Vector4 **out_rot, Vector3 **out_pos, const char **out_ecName, const EntityClass **out_ec) noexcept
	{
		return catchall([&] {
			static Vector4 rotCache;
			static Vector3 posCache;
			CheckPtr(instPtr, false);

			*out_name = instPtr->GetName().Buffer();
			*out_ecName = instPtr->GetEntityClassName().Buffer();
			*out_ec = instPtr->GetEntityClass();

			rotCache = instPtr->GetRotation();
			posCache = instPtr->GetPosition();

			*out_rot = &rotCache;
			*out_pos = &posCache;

			return true;
		});
	}

	uint8_t Instance_GetPropertyFromName(const Instance *instPtr, const char *propName, const char **out_value) noexcept
	{
		return catchall([&] {
			return GenericGetProperty(instPtr, propName, *out_value);
		});
	}

	uint8_t Instance_GetPropertyFromHash(const Instance *instPtr, uint32_t hashedPropName, const char **out_value) noexcept
	{
		return catchall([&] {
			return GenericGetProperty(instPtr, hashedPropName, *out_value);
		});
	}

	uint8_t Instance_GetPropertiesFromName(const Instance *instPtr, const char *propName, const char ***out_values, uint32_t *out_count) noexcept
	{
		return catchall([&] {
			return GenericGetProperties(instPtr, propName, *out_values, *out_count);
		});
	}

	uint8_t Instance_GetPropertiesFromHash(const Instance *instPtr, uint32_t hashedPropName, const char ***out_values, uint32_t *out_count) noexcept
	{
		return catchall([&] {
			return GenericGetProperties(instPtr, hashedPropName, *out_values, *out_count);
		});
	}

	void Instance_GetOverriddenProperties(const Instance *instPtr, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) noexcept
	{
		return catchall([&] {
			CheckPtr(instPtr)
			static List<String> values;
			static List<uint32_t> hashes;
			static List<const char*> ptrsBuffer;

			instPtr->GetOverriddenProperties(hashes, values);
			*out_hashesBuffer = hashes.GetArrayPtr();
			*out_count = (int32_t)values.Size();
			GetStringListPtrs(values, ptrsBuffer);
			*out_valuesBuffer = ptrsBuffer.GetArrayPtr();
		});
	}

	// Level //

	Level *Level_FromFile(const char *path) noexcept
	{
		return catchall([&] {
			CheckPtr(path, (Level *)nullptr);
			return Level::FromFile(path);
		});
	}

	Level *Level_FromStream(FileReader *stream) noexcept
	{
		return catchall([&] {
			CheckPtr(stream, (Level *)nullptr);
			return Level::FromStream(*stream);
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

	void Level_Destroy(Level *level) noexcept
	{
		catchall([&] {
			CheckPtr(level);
			Level::Destroy(level);
		});
	}

	uint8_t Level_IsWorldLevel(const Level *level) noexcept
	{
		return catchall([&] {
			CheckPtr(level, (uint8_t)0);
			return (uint8_t)level->IsWorldLevel();
		});
	}

	const void *Level_GetWrapper(const Level *level, uint32_t type, const char *name) noexcept
	{
		return catchall([&] {
			return Level_GetWrapperFNV(level, type, (uint32_t) FNV::Hash(name));
		});
	}

	const void *Level_GetWrapperFNV(const Level *level, uint32_t type, uint32_t name) noexcept
	{
		return catchall([&] {
			CheckPtr(level, (const void *)nullptr);

			switch (type)
			{
			case 0:
				return static_cast<const void*>(level->GetTerrain(name));
			case 1:
				return static_cast<const void*>(level->GetModel(name));
			case 2:
				return static_cast<const void*>(level->GetTexture(name));
			case 3:
				return static_cast<const void*>(level->GetWorld(name));
			case 4:
				return static_cast<const void*>(level->GetEntityClass(name));
			case 5:
				return static_cast<const void*>(level->GetAnimationBank(name));
			case 6:
				return static_cast<const void*>(level->GetScript(name));
			case 7:
				return static_cast<const void*>(level->GetSound(name));
			case 8:
				return static_cast<const void*>(level->GetLocalization(name));
			case 9:
				return static_cast<const void*>(level->GetAnimationSkeleton(name));
			case 10:
				return static_cast<const void*>(level->GetSoundBank(name));
			case 11:
				return static_cast<const void*>(level->GetSoundStream(name));
			default:
				return (const void *)nullptr;
			}
		});
	}

	const void *Level_GetWrappers(const Level *level, uint32_t type, uint32_t *out_numWrappers, uint32_t *out_wrapperSize) noexcept
	{
		return catchall([&] {
			*out_numWrappers = 0;
			*out_wrapperSize = 0;
			CheckPtr(level, (const void *)nullptr);

			switch (type)
			{
			case 0:
			{
				const List<Terrain>& terrains = level->GetTerrains();
				*out_numWrappers = (uint32_t)terrains.Size();
				*out_wrapperSize = sizeof(Terrain);
				return static_cast<const void*>(terrains.GetArrayPtr());
			}
			case 1:
			{
				const List<Model>& models = level->GetModels();
				*out_numWrappers = (uint32_t)models.Size();
				*out_wrapperSize = sizeof(Model);
				return static_cast<const void*>(models.GetArrayPtr());
			}
			case 2:
			{
				const List<Texture>& textures = level->GetTextures();
				*out_numWrappers = (uint32_t)textures.Size();
				*out_wrapperSize = sizeof(Texture);
				return static_cast<const void*>(textures.GetArrayPtr());
			}
			case 3:
			{
				const List<World>& worlds = level->GetWorlds();
				*out_numWrappers = (uint32_t)worlds.Size();
				*out_wrapperSize = sizeof(World);
				return static_cast<const void*>(worlds.GetArrayPtr());
			}
			case 4:
			{
				const List<EntityClass>& entityClasses = level->GetEntityClasses();
				*out_numWrappers = (uint32_t)entityClasses.Size();
				*out_wrapperSize = sizeof(EntityClass);
				return static_cast<const void*>(entityClasses.GetArrayPtr());
			}
			case 5:
			{
				const List<AnimationBank>& animationBanks = level->GetAnimationBanks();
				*out_numWrappers = (uint32_t)animationBanks.Size();
				*out_wrapperSize = sizeof(AnimationBank);
				return static_cast<const void*>(animationBanks.GetArrayPtr());
			}
			case 6:
			{
				const List<Script>& scripts = level->GetScripts();
				*out_numWrappers = (uint32_t)scripts.Size();
				*out_wrapperSize = sizeof(Script);
				return static_cast<const void*>(scripts.GetArrayPtr());
			}
			case 7:
			{
				const List<Sound>& sounds = level->GetSounds();
				*out_numWrappers = (uint32_t)sounds.Size();
				*out_wrapperSize = sizeof(Sound);
				return static_cast<const void*>(sounds.GetArrayPtr());
			}
			case 8:
			{
				const List<Localization>& locals = level->GetLocalizations();
				*out_numWrappers = (uint32_t)locals.Size();
				*out_wrapperSize = sizeof(Localization);
				return static_cast<const void*>(locals.GetArrayPtr());
			}
			case 9:
			{
				const List<AnimationSkeleton>& skels = level->GetAnimationSkeletons();
				*out_numWrappers = (uint32_t)skels.Size();
				*out_wrapperSize = sizeof(AnimationSkeleton);
				return static_cast<const void*>(skels.GetArrayPtr());
			}
			case 10:
			{
				const List<SoundBank>& banks = level->GetSoundBanks();
				*out_numWrappers = (uint32_t)banks.Size();
				*out_wrapperSize = sizeof(SoundBank);
				return static_cast<const void*>(banks.GetArrayPtr());
			}
			case 11:
			{
				const List<SoundStream>& streams = level->GetSoundStreams();
				*out_numWrappers = (uint32_t)streams.Size();
				*out_wrapperSize = sizeof(SoundStream);
				return static_cast<const void*>(streams.GetArrayPtr());
			}
			case 12:
			{
				const List<PlanSet>& planSets = level->GetPlanSets();
				*out_numWrappers = (uint32_t)planSets.Size();
				*out_wrapperSize = sizeof(PlanSet);
				return static_cast<const void*>(planSets.GetArrayPtr());
			}
			default:
				return (const void *)nullptr;
			}
		});
	}

	const char *Level_GetName(const Level *level) noexcept
	{
		return catchall([&] {
			static String cache;
			CheckPtr(level, (const char *)nullptr);
			cache = level->GetLevelName();
			return cache.Buffer();
		});
	}

	const char *Level_GetPath(const Level *level) noexcept
	{
		return catchall([&] {
			static String cache;
			CheckPtr(level, (const char *)nullptr);
			cache = level->GetLevelPath();
			return cache.Buffer();
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

	const Config **Level_GetConfigs(const Level *level, uint32_t header, int32_t *out_numConfigs) noexcept
	{
		return catchall([&] {
			static List<const Config *> configs;
			EConfigType cfgType = static_cast<EConfigType>(header);

			configs = level->GetConfigs(cfgType);

			*out_numConfigs = (uint32_t)configs.Size();
			return configs.GetArrayPtr();
		});
	}

	// Localization //

	const char *Localization_GetName(const Localization *local) noexcept
	{
		return catchall([&] {
			CheckPtr(local, (const char *)nullptr);
			return local->GetName().Buffer();
		});
	}

	uint8_t Localization_GetLocalizedWideString(const Localization *local, const char *path, uint16_t **out_chars, uint32_t *out_count) noexcept
	{
		return catchall([&] {
			CheckPtr(local, (uint8_t)0);
			return (uint8_t)local->GetLocalizedWideString(path, *out_chars, *out_count);
		});
	}

	// Logging //

	uint8_t LOG_GetNextLog(const char **out_msg, ELogType *out_level, uint32_t *out_line, const char **out_file) noexcept
	{
		return catchall([&] {
			static Logging::LoggerEntry current;
			bool hasLogEntry = Logging::Logger::GetNextLog(current);
			*out_msg = current.m_Message.Buffer();
			*out_level = current.m_Level;
			*out_line = current.m_Line;
			*out_file = current.m_File.Buffer();
			return hasLogEntry;
		});
	}

	void LOG_SetLogfileLevel(ELogType LogfileLevel) noexcept
	{
		catchall([&] {
			Logging::Logger::SetLogfileLevel(LogfileLevel);
		});
	}

	// Material //

	uint8_t Material_FetchAllFields(const Material *matPtr, Vector3 **out_specular, Vector3 **out_diffuse, char ***out_texPtrs, int32_t *out_numTexes, char **out_attachedLightName, uint32_t *out_matFlags, uint32_t *out_specExp, uint32_t *out_param1, uint32_t *out_param2) noexcept
	{
		return catchall([&] {
			static Vector3 specCache, diffCache;
			static char **texNamePtrsCache = new char*[4];
			static String namesCache[4];
			static char *attachedLightNameCache = nullptr;

			CheckPtr(matPtr, (uint8_t)0);

			*out_numTexes = 0;
			while (*out_numTexes < 4 && matPtr->GetTextureName(*out_numTexes, namesCache[*out_numTexes]))
			{
				texNamePtrsCache[*out_numTexes] = const_cast<char *>(namesCache[*out_numTexes].Buffer());
				*out_numTexes++;
			}

			*out_texPtrs = texNamePtrsCache;

			*out_specExp = matPtr->GetSpecularExponent();
			*out_matFlags = (uint32_t) matPtr->GetFlags();

			Color4u8 d = matPtr->GetDiffuseColor();
			Color4u8 s = matPtr->GetSpecularColor();

			diffCache = Vector3(d.m_Red, d.m_Green, d.m_Blue);
			specCache = Vector3(s.m_Red, s.m_Green, s.m_Blue);

			*out_diffuse = &diffCache;
			*out_specular = &specCache;

			*out_param1 = matPtr->GetFirstParameter();
			*out_param2 = matPtr->GetSecondParameter();

			*out_attachedLightName = const_cast<char *>(matPtr->GetAttachedLight().Buffer());

			return (uint8_t)1;
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

	uint8_t Model_FetchSimpleFields(const Model *model, const char **out_name, uint8_t *out_skinned, uint8_t *out_skelBroken, const Segment **out_segArr, int32_t *out_segCount, int32_t *out_segInc, const Bone **out_boneArr, int32_t *out_boneCount, int32_t *out_boneInc, const CollisionMesh **out_collMeshPtr) noexcept
	{
		return catchall([&] {
			static List<Bone> boneCache;

			CheckPtr(model, false);

			*out_name = model->GetName().Buffer();

			*out_skinned = model->IsSkeletalMesh();
			*out_skelBroken = model->IsSkeletonBroken();

			const List<Segment>& segs = model->GetSegments();
			*out_segArr = segs.GetArrayPtr();
			*out_segCount = (int32_t)segs.Size();
			*out_segInc = sizeof(Segment);

			*out_boneCount = 0;
			if (model->GetSkeleton(boneCache)) {
				*out_boneCount = (int32_t)boneCache.Size();
			}
			*out_boneArr = boneCache.GetArrayPtr();
			*out_boneInc = sizeof(Bone);

			*out_collMeshPtr = &(model->GetCollisionMesh());

			return true;
		});
	}

	void Model_GetPrimitivesMasked(const Model *model, uint32_t mask, int *out_numPrims, CollisionPrimitive ***out_primArrayPtr) noexcept
	{
		catchall([&] {
			static List<CollisionPrimitive> primsList;
			static List<CollisionPrimitive *> primPtrs;

			*out_numPrims = 0;
			CheckPtr(model);

			primsList = model->GetCollisionPrimitives((ECollisionMaskFlags) mask);
			primPtrs.Clear();

			for (size_t i = 0; i < primsList.Size(); i++) {
				primPtrs.Add(&primsList[i]);
			}

			*out_primArrayPtr = primPtrs.GetArrayPtr();
			*out_numPrims = (uint32_t) primPtrs.Size();
		});
	}

	// MODL //

	EModelPurpose MODL_GetPurpose(Chunks::MSH::MODL *modl) noexcept
	{
		return catchall([&] {
			CheckPtr(modl, EModelPurpose::Miscellaneous)
			return modl->GetPurpose();
		});
	}

	// MSH //

	Chunks::MSH::MSH *MSH_Create() noexcept
	{
		return catchall([&] {
			return Chunks::MSH::MSH::Create();
		});
	}

	uint8_t MSH_Delete(Chunks::MSH::MSH *msh) noexcept
	{
		return catchall([&] {
			CheckPtr(msh, false)
			LibSWBF2::Chunks::MSH::MSH::Destroy(msh);
			return true;
		});
	}

	uint8_t MSH_ReadFromFile(Chunks::MSH::MSH *msh, const char *path) noexcept
	{
		return catchall([&] {
			CheckPtr(msh, false)
			return msh->ReadFromFile(path);
		});
	}

	uint8_t MSH_WriteToFile(Chunks::MSH::MSH *msh, const char *path) noexcept
	{
		return catchall([&] {
			CheckPtr(msh, false)
			return msh->WriteToFile(path);
		});
	}

	// PlanSet //

	uint8_t PlanSet_GetChildWrappers(const PlanSet *ps, uint8_t id, void **out_listPtr, int32_t *out_listSize, int32_t *out_elSize) noexcept
	{
		return catchall([&] {
			CheckPtr(ps, false);

			*out_listSize = 0;
			*out_listPtr = nullptr;
			switch (id)
			{
				case 0:
				{
					const List<Hub>& hubs = ps->GetHubs();
					*out_listPtr = (void *)hubs.GetArrayPtr();
					*out_listSize = hubs.Size();
					*out_elSize = (int32_t) sizeof(Hub);
					break;
				}
				case 1:
				{
					const List<Connection>& cons = ps->GetConnections();
					*out_listPtr = (void *)cons.GetArrayPtr();
					*out_listSize = cons.Size();
					*out_elSize = (int32_t) sizeof(Connection);
					break;
				}
				default:
					return false;
			}

			return *out_listPtr != nullptr;
		});
	}

	// Region //

	uint8_t Region_FetchAllFields(const Region *regPtr, const Vector3 **out_size, const Vector3 **out_pos, const Vector4 **out_rot, const char **out_name, const char **out_type) noexcept
	{
		return catchall([&] {
			CheckPtr(regPtr, (uint8_t)0);
			static Vector4 rotCache;

			*out_size = &(regPtr->GetSize());
			*out_pos = &(regPtr->GetPosition());

			rotCache = regPtr->GetRotation();
			*out_rot = &rotCache;

			*out_name = regPtr->GetName().Buffer();
			*out_type = regPtr->GetType().Buffer();

			return (uint8_t)1;
		});
	}

	void Region_GetProperties(const Region *reg, uint32_t **out_hashesBuffer, const char ***out_valuesBuffer, int32_t *out_count) noexcept
	{
		catchall([&] {
			*out_count = 0;
			CheckPtr(reg)
			static List<const char*> ptrsBuffer;
			static List<String> values;
			static List<uint32_t> hashes;

			reg->GetProperties(hashes, values);

			*out_hashesBuffer = hashes.GetArrayPtr();
			*out_count = (int32_t)values.Size();
			GetStringListPtrs(values, ptrsBuffer);
			*out_valuesBuffer = ptrsBuffer.GetArrayPtr();
		});
	}

	// Scope //

	const Field **Scope_GetFields(Scope *ptr, uint32_t hash, uint32_t *out_count) noexcept
	{
		return catchall([&] {
			static List<const Field *> cache;
			cache = ptr->GetFields(hash);
			*out_count = (uint32_t)cache.Size();
			return cache.GetArrayPtr();
		});
	}

	// Script //

	LIBSWBF2_API const char *Script_GetName(const Script *script) noexcept
	{
		return catchall([&] {
			CheckPtr(script, (const char *)nullptr);
			static String nameCache;
			nameCache = script->GetName();
			return nameCache.Buffer();
		});
	}

	LIBSWBF2_API uint8_t Script_GetData(const Script *script, const uint8_t **out_data, uint32_t *out_size) noexcept
	{
		return catchall([&] {
			CheckPtr(script, (uint8_t)0);
			size_t sz;
			uint8_t res = (uint8_t)script->GetData(*out_data, sz);
			*out_size = (uint32_t)sz;
			return res;
		});
	}

	// Segment //

	uint8_t Segment_FetchAllFields(const Segment *seg, uint8_t *out_pretx, const char **out_boneName, const char **out_tag, uint32_t *out_numVerts, Vector3 **out_pBuf, Vector3 **out_nBuf, Vector2 **out_uvBuf, uint32_t *out_numVWs, VertexWeight **out_vwBuf, int32_t *out_topo, uint32_t *out_numInds, uint16_t **out_iBuf, const Material **out_mat) noexcept
	{
		return catchall([&] {
			static String boneNameCache;
			static String tagCache;

			CheckPtr(seg, false);

			*out_pretx = seg->IsPretransformed();

			boneNameCache = seg->GetBone();
			*out_boneName = boneNameCache.Buffer();

			tagCache = seg->GetTag();
			*out_tag = tagCache.Buffer();

			//Handle vertex buffers
			uint32_t numNormals, numUVs;
			seg->GetVertexBuffer(*out_numVerts, *out_pBuf);
			seg->GetNormalBuffer(numNormals, *out_nBuf);
			seg->GetUVBuffer(numUVs, *out_uvBuf);

			if (numUVs != numNormals || numNormals != *out_numVerts)
			{
				LOG_ERROR("Buffer length mismatch!");
				return false;
			}

			//Handle index buffer
			*out_topo = (int32_t) seg->GetTopology();
			seg->GetIndexBuffer(*out_numInds, *out_iBuf);

			//Handle weights
			*out_numVWs = seg->GetVertexWeights(*out_numVWs, *out_vwBuf) ? *out_numVWs : 0;

			//Material
			*out_mat = &(seg->GetMaterial());
			return true;
		});
	}

	// Sound //

	uint8_t Sound_GetData(const Sound *sound, uint32_t *out_sampleRate, uint32_t *out_sampleCount, uint8_t *out_blockAlign, const uint8_t **out_data) noexcept
	{
		return catchall([&] {
			CheckPtr(sound, (uint8_t)0);
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

	uint8_t Sound_FetchAllFields(const Sound *sound, uint32_t *out_format, int32_t *out_numChannels, int32_t *out_sampleRate, int32_t *out_numSamples, uint32_t *out_alias, uint8_t *out_hasData, uint32_t *out_name, uint32_t *out_numBytes) noexcept
	{
		return catchall([&] {
			CheckPtr(sound, false);
			*out_format = (uint32_t) sound->GetFormat();
			*out_numChannels = (int32_t) sound->GetNumChannels();
			*out_sampleRate = (int32_t) sound->GetSampleRate();
			*out_numSamples = (int32_t) sound->GetNumSamples();
			*out_alias = sound->GetAlias();
			*out_hasData = sound->HasData();
			*out_name = sound->GetHashedName();
			*out_numBytes = sound->GetDataLength();
			return true;
		});
	}

	// SoundBank //

	uint8_t SoundBank_FetchAllFields(const SoundBank *bnk, uint32_t *out_name, uint8_t *out_hasData, uint32_t *out_format) noexcept
	{
		return catchall([&] {
			CheckPtr(bnk, false);
			*out_hasData = bnk->HasData();
			*out_format = (uint32_t) bnk->GetFormat();
			*out_name = bnk->GetHashedName();
			return true;
		});
	}

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
			const List<Sound>& sounds = bnk->GetSounds();
			*out_sounds = sounds.GetArrayPtr();
			*out_numSounds = sounds.Size();
			*out_soundInc = sizeof(Sound);
			return *out_numSounds > 0;
		});
	}

	// SoundStream //

	uint8_t SoundStream_FetchAllFields(const SoundStream *str, uint32_t *out_name, uint8_t *out_hasData, uint32_t *out_format, uint32_t *out_numChannels, uint32_t *out_numSubstreams, uint32_t *out_substreamInterleave) noexcept
	{
		return catchall([&] {
			CheckPtr(str, false);
			*out_hasData = str->HasData();
			*out_format = (uint32_t) str->GetFormat();
			*out_name = str->GetHashedName();
			*out_numChannels = str->GetNumChannels();
			*out_numSubstreams = str->GetNumSubstreams();
			*out_substreamInterleave = str->GetSubstreamInterleave();
			return true;
		});
	}

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
			const List<Sound>& sounds = str->GetSounds();
			*out_sounds = sounds.GetArrayPtr();
			*out_numSounds = sounds.Size();
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
	uint8_t Terrain_FetchSimpleFields(const Terrain *ter, int32_t *out_numTexes, const char ***out_texNames, float *out_hU, float *out_hL, uint32_t *out_numVerts, Vector3 **out_vBuf, uint32_t *out_numNormals, Vector3 **out_nBuf, uint32_t *out_numUVs, Vector2 **out_uvBuf) noexcept
	{
		return catchall([&] {
			static List<const char*> texNamesPtrs;
			CheckPtr(ter, (uint8_t)0);

			const List<String>& texNames = ter->GetLayerTextures();
			*out_numTexes = (int32_t)texNames.Size();
			GetStringListPtrs(texNames, texNamesPtrs);
			*out_texNames = texNamesPtrs.GetArrayPtr();

			ter->GetHeightBounds(*out_hL, *out_hU);
			ter->GetVertexBuffer(*out_numVerts, *out_vBuf);
			ter->GetNormalBuffer(*out_numNormals, *out_nBuf);
			ter->GetUVBuffer(*out_numUVs, *out_uvBuf);

			return (uint8_t)1;
		});
	}

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

	void Terrain_GetIndexBuffer(const Terrain *terr, uint32_t **out_indicies, uint32_t *out_numInds) noexcept
	{
		catchall([&] {
			if (terr == nullptr || !terr->GetIndexBuffer(ETopology::TriangleList, *out_numInds, *out_indicies))
			{
				*out_numInds = 0;
			}
		});
	}

	// Texture //

	uint8_t Texture_FetchAllFields(const Texture *tex, uint16_t *out_width, uint16_t *out_height, const uint8_t **out_buf, const char **out_name) noexcept
	{
		return catchall([&] {
			static String nameCache;

			CheckPtr(tex, false);

			nameCache = tex->GetName();
			*out_name = nameCache.Buffer();
			return tex->GetImageData(ETextureFormat::R8_G8_B8_A8, 0, *out_width, *out_height, *out_buf);
		});
	}

	// World //

	uint8_t World_FetchAllFields(const World *world, const char **out_name, const char **out_skyName, const Terrain **out_terrPtr) noexcept
	{
		return catchall([&] {
			CheckPtr(world, false);
			static String skyNameCache;

			*out_name = world->GetName().Buffer();

			skyNameCache = world->GetSkyName();
			*out_skyName = skyNameCache.Buffer();

			*out_terrPtr = world->GetTerrain();

			return true;
		});
	}

	bool World_GetChildrenList(const World *world, uint8_t listID, void **out_listPtr, int32_t *out_listCount, int32_t *out_wrapperSize) noexcept
	{
		return catchall([&] {
			CheckPtr(world, false);

			*out_listPtr = nullptr;
			switch (listID)
			{
				case 0: {
					const List<Instance>& instances = world->GetInstances();
					*out_listPtr = (void *)instances.GetArrayPtr();
					*out_listCount = (int32_t)instances.Size();
					*out_wrapperSize = sizeof(Instance);
					break;
				}
				case 1: {
					const List<Region>& regions = world->GetRegions();
					*out_listPtr = regions.GetArrayPtr();
					*out_listCount = (int32_t)regions.Size();
					*out_wrapperSize = sizeof(Region);
					break;
				}
				case 2: {
					const List<WorldAnimation>& anims = world->GetAnimations();
					*out_listPtr = anims.GetArrayPtr();
					*out_listCount = (int32_t)anims.Size();
					*out_wrapperSize = sizeof(WorldAnimation);
					break;
				}
				case 3: {
					const List<WorldAnimationGroup>& animGroups = world->GetAnimationGroups();
					*out_listPtr = animGroups.GetArrayPtr();
					*out_listCount = (int32_t)animGroups.Size();
					*out_wrapperSize = sizeof(WorldAnimationGroup);
					break;
				}
				case 4: {
					const List<WorldAnimationHierarchy>& animHiers = world->GetAnimationHierarchies();
					*out_listPtr = animHiers.GetArrayPtr();
					*out_listCount = (int32_t)animHiers.Size();
					*out_wrapperSize = sizeof(WorldAnimationHierarchy);
					break;
				}
				case 5: {
					const List<Barrier>& barriers = world->GetBarriers();
					*out_listPtr = barriers.GetArrayPtr();
					*out_listCount = (int32_t)barriers.Size();
					*out_wrapperSize = sizeof(Barrier);
					break;
				}
				case 6: {
					const List<HintNode>& hintNodes = world->GetHintNodes();
					*out_listPtr = hintNodes.GetArrayPtr();
					*out_listCount = (int32_t)hintNodes.Size();
					*out_wrapperSize = sizeof(HintNode);
					break;
				}
				default:
					break;
			}

			return *out_listPtr != nullptr;
		});
	}

	// WorldAnimation //

	uint8_t WorldAnimation_FetchAllFields(const WorldAnimation *anim, uint8_t *out_loop, uint8_t *out_localT, const char **out_namePtr) noexcept
	{
		return catchall([&] {
			static String nameCache;
			CheckPtr(anim, false);

			*out_loop = anim->IsLooping();
			*out_localT = anim->IsTranslationLocal();

			nameCache = anim->GetName();
			*out_namePtr = nameCache.Buffer();

			return true;
		});
	}

	void WorldAnimation_GetAnimKeys(const WorldAnimation *anim, WorldAnimationKey **out_keyBuff, int32_t *out_numKeys, uint8_t IsRotation) noexcept
	{
		catchall([&] {
			static List<WorldAnimationKey> KeyCache;
			*out_numKeys = 0;
			CheckPtr(anim);

			if (IsRotation)
			{
				KeyCache = anim->GetRotationKeys();
			}
			else
			{
				KeyCache = anim->GetPositionKeys();
			}

			*out_keyBuff = KeyCache.GetArrayPtr();
			*out_numKeys = KeyCache.Size();
		});
	}

	// WorldAnimationGroup //

	uint8_t WorldAnimationGroup_FetchAllFields(const WorldAnimationGroup *group, uint8_t *out_bool0, uint8_t *out_bool1, const char **out_namePtr) noexcept
	{
		return catchall([&] {
			static String nameCache;
			CheckPtr(group, false);

			nameCache = group->GetName();
			*out_namePtr = nameCache.Buffer();

			*out_bool0 = group->IsPlayingAtStart();
			*out_bool1 = group->DisablesHierarchies();

			return true;
		});
	}

	void WorldAnimationGroup_GetAnimInstPairs(const WorldAnimationGroup *group, const char ***out_animNames, const char ***out_instNames, int32_t *out_numPairs) noexcept
	{
		catchall([&] {
			static List<String> animsCache;
			static List<String> instsCache;
			static List<const char*> animsPtrsBuffer;
			static List<const char*> instsPtrsBuffer;

			*out_numPairs = 0;
			CheckPtr(group);

			group->GetAnimationInstancePairs(animsCache, instsCache);

			*out_numPairs = (int32_t)animsCache.Size();

			GetStringListPtrs(animsCache, animsPtrsBuffer);
			GetStringListPtrs(instsCache, instsPtrsBuffer);

			*out_animNames = animsPtrsBuffer.GetArrayPtr();
			*out_instNames = instsPtrsBuffer.GetArrayPtr();
		});
	}

	// WorldAnimationHierarchy //

	uint8_t WorldAnimationHierarchy_FetchAllFields(const WorldAnimationHierarchy *hier, const char  **out_rootPtr, const char ***out_childNames, int32_t *out_numChildren) noexcept
	{
		return catchall([&] {
			static String rootCache;
			static List<String> childrenCache;
			static List<const char*> childPtrsBuffer;
			CheckPtr(hier, false);

			rootCache = hier->GetRootName();
			*out_rootPtr = rootCache.Buffer();

			childrenCache = hier->GetChildNames();
			GetStringListPtrs(childrenCache, childPtrsBuffer);

			*out_childNames = childPtrsBuffer.GetArrayPtr();
			*out_numChildren = childrenCache.Size();

			return true;
		});
	}
}
