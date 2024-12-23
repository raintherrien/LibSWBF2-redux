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
#define CHECKNULL(PTR, ...) if (nullptr == (PTR)) { LIBSWBF2_LOG_ERROR("Given Pointer was NULL!"); return __VA_ARGS__; }

	template<typename T> auto catchall(T &&t) noexcept -> decltype(t())
	{
		try {
			return t();
		} catch (const std::exception &e) {
			LIBSWBF2_LOG_ERROR("{}", e.what());
		} catch (...) {
			LIBSWBF2_LOG_ERROR("Unknown exception");
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

	// API List //

	struct CList {
		virtual ~CList() noexcept { }
		virtual void *at(size_t index) noexcept = 0;
		virtual size_t size() noexcept = 0;
	};

	template<typename T>
	struct TypedList : CList {
		std::vector<T> data;

		TypedList(std::vector<T> &&d) : data{std::forward<std::vector<T>>(d)} { }
		TypedList(const std::vector<T> &d) : data{d} { }
		~TypedList() noexcept { }

		void *at(size_t index) noexcept override
		{
			if (index < data.size()) {
				LIBSWBF2_LOG_INFO("TypedList::at({}) -> {}", index, fmt::ptr(&data[index]));
				return &data[index];
			} else {
				LIBSWBF2_LOG_INFO("TypedList::at({}) > size {} -> nullptr", index, data.size());
				return nullptr;
			}
		}

		size_t size() noexcept override
		{
			return data.size();
		}
	};

	size_t CList_size(CList *l) noexcept
	{
		LIBSWBF2_LOG_INFO("CList_size({})", fmt::ptr(l));
		return l->size();
	}

	void CList_free(CList *l) noexcept
	{
		LIBSWBF2_LOG_INFO("CList_free({})", fmt::ptr(l));
		delete l;
	}

	void *CList_at(CList *l, size_t i) noexcept
	{
		LIBSWBF2_LOG_INFO("CList_at({}, {})", fmt::ptr(l), i);
		return l->at(i);
	}

	// C API smart pointer wrappers //

	struct Container_Owned { std::shared_ptr<LibSWBF2::Container> ptr; };
	struct Level_Owned { std::shared_ptr<LibSWBF2::Level> ptr; };

	// Bone //

	Vector3 Bone_GetPosition(const Bone *bone) noexcept
	{
		LIBSWBF2_LOG_INFO("Bone_GetPosition({})", fmt::ptr(bone));
		return catchall([&] {
			CHECKNULL(bone, Vector3{});
			return bone->m_Position;
		});
	}

	Vector4 Bone_GetRotation(const Bone *bone) noexcept
	{
		LIBSWBF2_LOG_INFO("Bone_GetRotation({})", fmt::ptr(bone));
		return catchall([&] {
			CHECKNULL(bone, Vector4{});
			return bone->m_Rotation;
		});
	}

	size_t Bone_GetName(const Bone *bone, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("Bone_GetName({}, {}, {})", fmt::ptr(bone), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(bone, static_cast<size_t>(0));
			std::string name = bone->m_BoneName;
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	size_t Bone_GetParentName(const Bone *bone, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("Bone_GetParentName({}, {}, {})", fmt::ptr(bone), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(bone, static_cast<size_t>(0));
			std::string parent_name = bone->m_Parent;
			copy_c_str(parent_name, out_buffer, out_buffer_len);
			return parent_name.size();
		});
	}

	std::string Bone_GetNameS(const Bone *bone) noexcept
	{
		LIBSWBF2_LOG_INFO("Bone_GetNameS({})", fmt::ptr(bone));
		return catchall([&] {
			CHECKNULL(bone, std::string{});
			return bone->m_BoneName;
		});
	}

	std::string Bone_GetParentNameS(const Bone *bone) noexcept
	{
		LIBSWBF2_LOG_INFO("Bone_GetParentNameS({})", fmt::ptr(bone));
		return catchall([&] {
			CHECKNULL(bone, std::string{});
			return bone->m_Parent;
		});
	}

	// Collision Mesh //

	CList *CollisionMesh_GetIndexBuffer(const CollisionMesh *mesh) noexcept
	{
		LIBSWBF2_LOG_INFO("CollisionMesh_GetIndexBuffer({})", fmt::ptr(mesh));
		return catchall([&] {
			CHECKNULL(mesh, static_cast<TypedList<uint16_t> *>(nullptr));
			return new TypedList<uint16_t>{mesh->GetIndexBuffer()};
		});
	}

	CList *CollisionMesh_GetVertexBuffer(const CollisionMesh *mesh) noexcept
	{
		LIBSWBF2_LOG_INFO("CollisionMesh_GetVertexBuffer({})", fmt::ptr(mesh));
		return catchall([&] {
			CHECKNULL(mesh, static_cast<TypedList<Vector3> *>(nullptr));
			return new TypedList<Vector3>{mesh->GetVertexBuffer()};
		});
	}

	TList<uint16_t> CollisionMesh_GetIndexBufferT(const CollisionMesh *mesh) noexcept
	{
		return TList<uint16_t>::create(CollisionMesh_GetIndexBuffer(mesh));
	}

	TList<Vector3> CollisionMesh_GetVertexBufferT(const CollisionMesh *mesh) noexcept
	{
		return TList<Vector3>::create(CollisionMesh_GetVertexBuffer(mesh));
	}

	// CollisionPrimitive //

	Vector3 CollisionPrimitive_GetPosition(const CollisionPrimitive *cp) noexcept
	{
		LIBSWBF2_LOG_INFO("CollisionPrimitive_GetPosition({})", fmt::ptr(cp));
		return catchall([&] {
			CHECKNULL(cp, Vector3{});
			return cp->GetPosition();
		});
	}

	Vector4 CollisionPrimitive_GetRotation(const CollisionPrimitive *cp) noexcept
	{
		LIBSWBF2_LOG_INFO("CollisionPrimitive_GetRotation({})", fmt::ptr(cp));
		return catchall([&] {
			CHECKNULL(cp, Vector4{});
			return cp->GetRotation();
		});
	}

	size_t CollisionPrimitive_GetParentName(const CollisionPrimitive *cp, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("CollisionPrimitive_GetParentName({}, {}, {})", fmt::ptr(cp), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(cp, static_cast<size_t>(0));
			std::string parent_name = cp->GetParentName();
			copy_c_str(parent_name, out_buffer, out_buffer_len);
			return parent_name.size();
		});
	}

	ECollisionPrimitiveType CollisionPrimitive_GetType(const CollisionPrimitive *cp) noexcept
	{
		LIBSWBF2_LOG_INFO("CollisionPrimitive_GetType({})", fmt::ptr(cp));
		return catchall([&] {
			CHECKNULL(cp, static_cast<ECollisionPrimitiveType>(0));
			return cp->GetPrimitiveType();
		});
	}

	void CollisionPrimitive_GetCubeDims(const CollisionPrimitive *cp, float *sx, float *sy, float *sz) noexcept
	{
		LIBSWBF2_LOG_INFO("CollisionPrimitive_GetCubeDims({}, {}, {}, {})", fmt::ptr(cp), fmt::ptr(sx), fmt::ptr(sy), fmt::ptr(sz));
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
		LIBSWBF2_LOG_INFO("CollisionPrimitive_GetCylinderDims({}, {}, {})", fmt::ptr(cp), fmt::ptr(sr), fmt::ptr(sh));
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
		LIBSWBF2_LOG_INFO("CollisionPrimitive_GetSphereDims({}, {})", fmt::ptr(cp), fmt::ptr(sr));
		catchall([&] {
			if (cp == nullptr) {
				*sr = 0.0f;
			} else {
				cp->GetSphereRadius(*sr);
			}
		});
	}

	std::string CollisionPrimitive_GetParentNameS(const CollisionPrimitive *cp) noexcept
	{
		LIBSWBF2_LOG_INFO("CollisionPrimitive_GetParentNameS({})", fmt::ptr(cp));
		return catchall([&] {
			CHECKNULL(cp, std::string{});
			return cp->GetParentName();
		});
	}

	// Config //

	const Field *Config_GetField(const Config *cfg, uint32_t name_hash) noexcept
	{
		LIBSWBF2_LOG_INFO("Config_GetField({}, {})", fmt::ptr(cfg), name_hash);
		return catchall([&] {
			CHECKNULL(cfg, static_cast<const Field *>(nullptr));
			return cfg->GetField(name_hash);
		});
	}

	CList *Config_GetFields(const Config *cfg, uint32_t name_hash) noexcept
	{
		LIBSWBF2_LOG_INFO("Config_GetFields({}, {})", fmt::ptr(cfg), name_hash);
		return catchall([&] {
			CHECKNULL(cfg, static_cast<TypedList<const Field *> *>(nullptr));
			return new TypedList<const Field *>{cfg->GetFields(name_hash)};
		});
	}

	TList<const Field *> Config_GetFieldsT(const Config *cfg, uint32_t name_hash) noexcept
	{
		return TList<const Field *>::create(Config_GetFields(cfg, name_hash));
	}

	// Container //

	Container_Owned *Container_Create() noexcept
	{
		LIBSWBF2_LOG_INFO("Container_Create()");
		return catchall([&] {
			if (auto c = std::make_shared<LibSWBF2::Container>()) {
				return new Container_Owned{c};
			}
			return static_cast<Container_Owned *>(nullptr);
		});
	}

	void Container_Destroy(Container_Owned *container) noexcept
	{
		LIBSWBF2_LOG_INFO("Container_Destroy({})", fmt::ptr(container));
		catchall([&] {
			if (container) {
				delete container;
			}
		});
	}

	Level_Owned *Container_AddLevel(const Container_Owned *container, const char *lvl_path) noexcept
	{
		LIBSWBF2_LOG_INFO("Container_AddLevel({}, '{}')", fmt::ptr(container), lvl_path);
		return catchall([&] {
			CHECKNULL(container, static_cast<Level_Owned *>(nullptr))
			if (auto l = container->ptr->AddLevel(lvl_path)) {
				return new Level_Owned{l};
			}
			return static_cast<Level_Owned *>(nullptr);
		});
	}

	const Config *Container_FindConfig(const Container_Owned *container, EConfigType type, uint32_t name_hash) noexcept
	{
		LIBSWBF2_LOG_INFO("Container_FindConfig({}, {}, {})", fmt::ptr(container), ConfigTypeToString(type), name_hash);
		return catchall([&] {
			CHECKNULL(container, static_cast<const Config *>(nullptr));
			return container->ptr->FindConfig(type, name_hash);
		});
	}

	const Texture *Container_FindTexture(const Container_Owned *container, uint32_t name_hash) noexcept
	{
		LIBSWBF2_LOG_INFO("Container_FindTexture({}, {})", fmt::ptr(container), name_hash);
		return catchall([&] {
			CHECKNULL(container, static_cast<const Texture *>(nullptr));
			return container->ptr->FindTexture(name_hash);
		});
	}

	const Model *Container_FindModel(const Container_Owned *container, uint32_t name_hash) noexcept
	{
		LIBSWBF2_LOG_INFO("Container_FindModel({}, {})", fmt::ptr(container), name_hash);
		return catchall([&] {
			CHECKNULL(container, static_cast<const Model *>(nullptr));
			return container->ptr->FindModel(name_hash);
		});
	}

	const EntityClass *Container_FindEntityClass(const Container_Owned *container, uint32_t name_hash) noexcept
	{
		LIBSWBF2_LOG_INFO("Container_FindEntityClass({}, {})", fmt::ptr(container), name_hash);
		return catchall([&] {
			CHECKNULL(container, static_cast<const EntityClass *>(nullptr));
			return container->ptr->FindEntityClass(name_hash);
		});
	}

	// EntityClass //

	size_t EntityClass_GetBaseName(const EntityClass *ec, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("EntityClass_GetBaseName({}, {}, {})", fmt::ptr(ec), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(ec, static_cast<size_t>(0));
			std::string base_name = ec->GetBaseName();
			copy_c_str(base_name, out_buffer, out_buffer_len);
			return base_name.size();
		});
	}

	size_t EntityClass_GetPropertyValue(const EntityClass *ec, uint32_t name_hash, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("EntityClass_GetPropertyValue({}, {}, {}, {})", fmt::ptr(ec), name_hash, fmt::ptr(out_buffer), out_buffer_len);
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

	CList *EntityClass_GetAllPropertyHashes(const EntityClass *ec) noexcept
	{
		LIBSWBF2_LOG_INFO("EntityClass_GetAllPropertyHashes({})", fmt::ptr(ec));
		return catchall([&] {
			CHECKNULL(ec, static_cast<TypedList<uint32_t> *>(nullptr));
			return new TypedList<uint32_t>{ec->GetAllPropertyHashes()};
		});
	}

	std::string EntityClass_GetBaseNameS(const EntityClass *ec) noexcept
	{
		LIBSWBF2_LOG_INFO("EntityClass_GetBaseNames({})", fmt::ptr(ec));
		return catchall([&] {
			CHECKNULL(ec, std::string{});
			return ec->GetBaseName();
		});
	}

	std::string EntityClass_GetPropertyValueS(const EntityClass *ec, uint32_t name_hash) noexcept
	{
		LIBSWBF2_LOG_INFO("EntityClass_GetPropertyValueS({}, {})", fmt::ptr(ec), name_hash);
		return catchall([&] {
			CHECKNULL(ec, std::string{});
			std::string value;
			if (!ec->GetProperty(name_hash, value)) {
				return std::string{};
			}
			return value;
		});
	}

	TList<uint32_t> EntityClass_GetAllPropertyHashesT(const EntityClass *ec) noexcept
	{
		return TList<uint32_t>::create(EntityClass_GetAllPropertyHashes(ec));
	}

	// Field //

	const Scope *Field_GetScope(const Field *field) noexcept
	{
		LIBSWBF2_LOG_INFO("Field_GetScope({})", fmt::ptr(field));
		return catchall([&] {
			CHECKNULL(field, static_cast<const Scope *>(nullptr));
			return &field->m_Scope;
		});
	}

	size_t Field_GetString(const Field *field, size_t index, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("Field_GetString({}, {}, {}, {})", fmt::ptr(field), index, fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(field, static_cast<size_t>(0));
			std::string s = field->GetString(index);
			copy_c_str(s, out_buffer, out_buffer_len);
			return s.size();
		});
	}

	// Helpers //

	uint32_t FNVHashString(const char *string) noexcept
	{
		LIBSWBF2_LOG_INFO("FNVHashString('{}')", string);
		return catchall([&] {
			return FNV::Hash(string);
		});
	}

	// Instance //

	size_t Instance_GetName(const Instance *instance, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("Instance_GetName({}, {}, {})", fmt::ptr(instance), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(instance, static_cast<size_t>(0));
			std::string name = instance->GetName();
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	size_t Instance_GetEntityClassName(const Instance *instance, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("Instance_GetEntityClassName({}, {}, {})", fmt::ptr(instance), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(instance, static_cast<size_t>(0));
			std::string name = instance->GetEntityClassName();
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	Vector3 Instance_GetPosition(const Instance *instance) noexcept
	{
		LIBSWBF2_LOG_INFO("Instance_GetPosition({})", fmt::ptr(instance));
		return catchall([&] {
			CHECKNULL(instance, Vector3{});
			return instance->GetPosition();
		});
	}

	Vector4 Instance_GetRotation(const Instance *instance) noexcept
	{
		LIBSWBF2_LOG_INFO("Instance_GetRotation({})", fmt::ptr(instance));
		return catchall([&] {
			CHECKNULL(instance, Vector4{});
			return instance->GetRotation();
		});
	}

	std::string Instance_GetNameS(const Instance *instance) noexcept
	{
		LIBSWBF2_LOG_INFO("Instance_GetNameS({})", fmt::ptr(instance));
		return catchall([&] {
			CHECKNULL(instance, std::string{});
			return instance->GetName();
		});
	}

	std::string Instance_GetEntityClassNameS(const Instance *instance) noexcept
	{
		LIBSWBF2_LOG_INFO("Instance_GetEntityClassNameS({})", fmt::ptr(instance));
		return catchall([&] {
			CHECKNULL(instance, std::string{});
			return instance->GetEntityClassName();
		});
	}

	// Level //

	void Level_Destroy(Level_Owned *level) noexcept
	{
		delete level;
	}

	bool Level_IsWorldLevel(const Level_Owned *level) noexcept
	{
		LIBSWBF2_LOG_INFO("Level_IsWorldLevel({})", fmt::ptr(level));
		return catchall([&] {
			CHECKNULL(level, false);
			return level->ptr->IsWorldLevel();
		});
	}

	size_t Level_GetName(const Level_Owned *level, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("Level_GetName({}, {}, {})", fmt::ptr(level), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(level, static_cast<size_t>(0));
			std::string level_name = level->ptr->GetLevelName();
			copy_c_str(level_name, out_buffer, out_buffer_len);
			return level_name.size();
		});
	}

	CList *Level_GetWorlds(const Level_Owned *level) noexcept
	{
		LIBSWBF2_LOG_INFO("Level_GetWorlds({})", fmt::ptr(level));
		return catchall([&] {
			CHECKNULL(level, static_cast<TypedList<World> *>(nullptr));
			return new TypedList<World>{level->ptr->GetWorlds()};
		});
	}

	TList<const World> Level_GetWorldsT(const Level_Owned *level) noexcept
	{
		return TList<const World>::create(Level_GetWorlds(level));
	}

	// Material //

	const Texture *Material_GetTexture(const Material *mat, size_t index) noexcept
	{
		LIBSWBF2_LOG_INFO("Material_GetTexture({}, {})", fmt::ptr(mat), index);
		return catchall([&] {
			CHECKNULL(mat, static_cast<const Texture *>(nullptr));
			return mat->GetTexture(index);
		});
	}

	EMaterialFlags Material_GetFlags(const Material *mat) noexcept
	{
		LIBSWBF2_LOG_INFO("Material_GetFlags({})", fmt::ptr(mat));
		return catchall([&] {
			CHECKNULL(mat, static_cast<EMaterialFlags>(0));
			return mat->GetFlags();
		});
	}

	// Model //

	const CollisionMesh *Model_GetCollisionMesh(const Model *model) noexcept
	{
		LIBSWBF2_LOG_INFO("Model_GetCollisionMesh({})", fmt::ptr(model));
		return catchall([&] {
			CHECKNULL(model, static_cast<const CollisionMesh *>(nullptr));
			return model->GetCollisionMesh();
		});
	}

	CList *Model_GetSegments(const Model *model) noexcept
	{
		LIBSWBF2_LOG_INFO("Model_GetSegments({})", fmt::ptr(model));
		return catchall([&] {
			CHECKNULL(model, static_cast<TypedList<Segment> *>(nullptr));
			return new TypedList<Segment>{model->GetSegments()};
		});
	}

	CList *Model_GetBones(const Model *model) noexcept
	{
		LIBSWBF2_LOG_INFO("Model_GetBones({})", fmt::ptr(model));
		return catchall([&] {
			CHECKNULL(model, static_cast<TypedList<Bone> *>(nullptr));
			return new TypedList<Bone>{model->GetBones()};
		});
	}

	CList *Model_GetCollisionPrimitives(const Model *model) noexcept
	{
		LIBSWBF2_LOG_INFO("Model_GetCollisionPrimitives({})", fmt::ptr(model));
		return catchall([&] {
			CHECKNULL(model, static_cast<TypedList<CollisionPrimitive> *>(nullptr));
			return new TypedList<CollisionPrimitive>{model->GetCollisionPrimitives()};
		});
	}

	TList<const Segment> Model_GetSegmentsT(const Model *model) noexcept
	{
		return TList<const Segment>::create(Model_GetSegments(model));
	}

	TList<const Bone> Model_GetBonesT(const Model *model) noexcept
	{
		return TList<const Bone>::create(Model_GetBones(model));
	}

	TList<const CollisionPrimitive> Model_GetCollisionPrimitivesT(const Model *model) noexcept
	{
		return TList<const CollisionPrimitive>::create(Model_GetCollisionPrimitives(model));
	}

	// Scope //

	const Field *Scope_GetField(const Scope *scope, uint32_t name_hash) noexcept
	{
		LIBSWBF2_LOG_INFO("Scope_GetField({}, {})", fmt::ptr(scope), name_hash);
		return catchall([&] {
			CHECKNULL(scope, static_cast<const Field *>(nullptr));
			return scope->GetField(name_hash);
		});
	}

	CList *Scope_GetFields(const Scope *scope, uint32_t name_hash) noexcept
	{
		LIBSWBF2_LOG_INFO("Scope_GetFields({}, {})", fmt::ptr(scope), name_hash);
		return catchall([&] {
			CHECKNULL(scope, static_cast<TypedList<const Field *> *>(nullptr));
			return new TypedList<const Field *>{scope->GetFields(name_hash)};
		});
	}

	TList<const Field *> Scope_GetFieldsT(const Scope *scope, uint32_t name_hash) noexcept
	{
		return TList<const Field *>::create(Scope_GetFields(scope, name_hash));
	}

	// Segment //

	ETopology Segment_GetTopology(const Segment *seg) noexcept
	{
		LIBSWBF2_LOG_INFO("Segment_GetTopology({})", fmt::ptr(seg));
		return catchall([&] {
			CHECKNULL(seg, static_cast<ETopology>(0));
			return seg->GetTopology();
		});
	}

	size_t Segment_GetBoneName(const Segment *seg, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("Segment_GetBoneName({}, {}, {})", fmt::ptr(seg), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(seg, static_cast<size_t>(0));
			std::string bone_name = seg->GetBone();
			copy_c_str(bone_name, out_buffer, out_buffer_len);
			return bone_name.size();
		});
	}

	const Material *Segment_GetMaterial(const Segment *seg) noexcept
	{
		LIBSWBF2_LOG_INFO("Segment_GetMaterial({})", fmt::ptr(seg));
		return catchall([&] {
			CHECKNULL(seg, static_cast<const Material *>(nullptr));
			return seg->GetMaterial();
		});
	}

	CList *Segment_GetIndexBuffer(const Segment *seg) noexcept
	{
		LIBSWBF2_LOG_INFO("Segment_GetIndexBuffer({})", fmt::ptr(seg));
		return catchall([&] {
			CHECKNULL(seg, static_cast<TypedList<uint16_t> *>(nullptr));
			return new TypedList<uint16_t>{seg->GetIndexBuffer()};
		});
	}

	CList *Segment_GetVertexBuffer(const Segment *seg) noexcept
	{
		LIBSWBF2_LOG_INFO("Segment_GetVertexBuffer({})", fmt::ptr(seg));
		return catchall([&] {
			CHECKNULL(seg, static_cast<TypedList<Vector3> *>(nullptr));
			return new TypedList<Vector3>{seg->GetVertexBuffer()};
		});
	}

	CList *Segment_GetUVBuffer(const Segment *seg) noexcept
	{
		LIBSWBF2_LOG_INFO("Segment_GetUVBuffer({})", fmt::ptr(seg));
		return catchall([&] {
			CHECKNULL(seg, static_cast<TypedList<Vector2> *>(nullptr));
			return new TypedList<Vector2>{seg->GetUVBuffer()};
		});
	}

	CList *Segment_GetNormalBuffer(const Segment *seg) noexcept
	{
		LIBSWBF2_LOG_INFO("Segment_GetNormalBuffer({})", fmt::ptr(seg));
		return catchall([&] {
			CHECKNULL(seg, static_cast<TypedList<Vector3> *>(nullptr));
			return new TypedList<Vector3>{seg->GetNormalBuffer()};
		});
	}

	std::string Segment_GetBoneNameS(const Segment *seg) noexcept
	{
		LIBSWBF2_LOG_INFO("Segment_GetBoneNameS({})", fmt::ptr(seg));
		return catchall([&] {
			CHECKNULL(seg, std::string{});
			return seg->GetBone();
		});
	}

	TList<uint16_t> Segment_GetIndexBufferT(const Segment *seg) noexcept
	{
		return TList<uint16_t>::create(Segment_GetIndexBuffer(seg));
	}

	TList<const Vector3> Segment_GetVertexBufferT(const Segment *seg) noexcept
	{
		return TList<const Vector3>::create(Segment_GetVertexBuffer(seg));
	}

	TList<const Vector2> Segment_GetUVBufferT(const Segment *seg) noexcept
	{
		return TList<const Vector2>::create(Segment_GetUVBuffer(seg));
	}

	TList<const Vector3> Segment_GetNormalBufferT(const Segment *seg) noexcept
	{
		return TList<const Vector3>::create(Segment_GetNormalBuffer(seg));
	}

	// Terrain //

	CList *Terrain_GetIndexBuffer(const Terrain *terr) noexcept
	{
		LIBSWBF2_LOG_INFO("Terrain_GetIndexBuffer({})", fmt::ptr(terr));
		return catchall([&] {
			CHECKNULL(terr, static_cast<TypedList<uint32_t> *>(nullptr));
			return new TypedList<uint32_t>{terr->GetIndexBuffer(ETopology::TriangleList)};
		});
	}

	CList *Terrain_GetVertexBuffer(const Terrain *terr) noexcept
	{
		LIBSWBF2_LOG_INFO("Terrain_GetVertexBuffer({})", fmt::ptr(terr));
		return catchall([&] {
			CHECKNULL(terr, static_cast<TypedList<Vector3> *>(nullptr));
			return new TypedList<Vector3>{terr->GetVertexBuffer()};
		});
	}

	CList *Terrain_GetUVBuffer(const Terrain *terr) noexcept
	{
		LIBSWBF2_LOG_INFO("Terrain_GetUVBuffer({})", fmt::ptr(terr));
		return catchall([&] {
			CHECKNULL(terr, static_cast<TypedList<Vector2> *>(nullptr));
			return new TypedList<Vector2>{terr->GetUVBuffer()};
		});
	}

	CList *Terrain_GetBlendMap(const Terrain *terr, uint32_t *out_dim, uint32_t *out_num_layers) noexcept
	{
		LIBSWBF2_LOG_INFO("Terrain_GetBlendMap({}, {}, {})", fmt::ptr(terr), fmt::ptr(out_dim), fmt::ptr(out_num_layers));
		return catchall([&] {
			*out_dim = 0;
			*out_num_layers = 0;
			CHECKNULL(terr, static_cast<TypedList<uint8_t> *>(nullptr));
			return new TypedList<uint8_t>{terr->GetBlendMap(*out_dim, *out_num_layers)};
		});
	}

	CList *Terrain_GetLayerTextures(const Terrain *terr, const Container_Owned *container) noexcept
	{
		LIBSWBF2_LOG_INFO("Terrain_GetLayerTextures({}, {})", fmt::ptr(terr), fmt::ptr(container));
		return catchall([&] {
			CHECKNULL(terr, static_cast<TypedList<Texture> *>(nullptr));
			CHECKNULL(container, static_cast<TypedList<Texture> *>(nullptr));
			std::vector<std::string> layer_texture_names = terr->GetLayerTextures();
			std::vector<Texture> layer_textures;
			for (const std::string &texture_name : layer_texture_names) {
				layer_textures.emplace_back(*container->ptr->FindTexture(texture_name));
			}
			return new TypedList<Texture>{layer_textures};
		});
	}

	TList<uint32_t> Terrain_GetIndexBufferT(const Terrain *terr) noexcept
	{
		return TList<uint32_t>::create(Terrain_GetIndexBuffer(terr));
	}

	TList<const Vector3> Terrain_GetVertexBufferT(const Terrain *terr) noexcept
	{
		return TList<const Vector3>::create(Terrain_GetVertexBuffer(terr));
	}

	TList<const Vector2> Terrain_GetUVBufferT(const Terrain *terr) noexcept
	{
		return TList<const Vector2>::create(Terrain_GetUVBuffer(terr));
	}

	TList<uint8_t> Terrain_GetBlendMapT(const Terrain *terr, uint32_t *out_width, uint32_t *out_num_layers) noexcept
	{
		return TList<uint8_t>::create(Terrain_GetBlendMap(terr, out_width, out_num_layers));
	}

	TList<const Texture> Terrain_GetLayerTexturesT(const Terrain *terr, const Container_Owned *container) noexcept
	{
		return TList<const Texture>::create(Terrain_GetLayerTextures(terr, container));
	}

	// Texture //

	size_t Texture_GetName(const Texture *tex, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("Texture_GetName({}, {}, {})", fmt::ptr(tex), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(tex, static_cast<size_t>(0));
			std::string name = tex->GetName();
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	CList *Texture_GetData(const Texture *tex, uint16_t *out_width, uint16_t *out_height) noexcept
	{
		LIBSWBF2_LOG_INFO("Texture_GetData({}, {}, {})", fmt::ptr(tex), fmt::ptr(out_width), fmt::ptr(out_height));
		return catchall([&] {
			if (tex == nullptr) {
				*out_width = 0;
				*out_height = 0;
				return static_cast<TypedList<uint8_t> *>(nullptr);
			}
			const uint8_t *buff = nullptr;
			tex->GetImageData(ETextureFormat::R8_G8_B8_A8, 0, *out_width, *out_height, buff);
			size_t len = *out_width * *out_height * 4;
			return new TypedList<uint8_t>{std::vector<uint8_t>{buff, buff + len}};
		});
	}

	std::string Texture_GetNameS(const Texture *tex) noexcept
	{
		LIBSWBF2_LOG_INFO("Texture_GetNameS({})", fmt::ptr(tex));
		return catchall([&] {
			CHECKNULL(tex, std::string{});
			return tex->GetName();
		});
	}

	TList<uint8_t> Texture_GetDataT(const Texture *tex, uint16_t *out_width, uint16_t *out_height) noexcept
	{
		return TList<uint8_t>::create(Texture_GetData(tex, out_width, out_height));
	}

	// World //

	size_t World_GetName(const World *world, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("World_GetName({}, {}, {})", fmt::ptr(world), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(world, static_cast<size_t>(0));
			std::string name = world->GetName();
			copy_c_str(name, out_buffer, out_buffer_len);
			return name.size();
		});
	}

	size_t World_GetSkyName(const World *world, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("World_GetSkyName({}, {}, {})", fmt::ptr(world), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(world, static_cast<size_t>(0));
			std::string sky_name = world->GetSkyName();
			copy_c_str(sky_name, out_buffer, out_buffer_len);
			return sky_name.size();
		});
	}

	CList *World_GetInstances(const World *world) noexcept
	{
		LIBSWBF2_LOG_INFO("World_GetInstance({})", fmt::ptr(world));
		return catchall([&] {
			CHECKNULL(world, static_cast<TypedList<Instance> *>(nullptr));
			return new TypedList<Instance>{world->GetInstances()};
		});
	}

	std::string World_GetNameS(const World *world) noexcept
	{
		LIBSWBF2_LOG_INFO("World_GetNameS({})", fmt::ptr(world));
		return catchall([&] {
			CHECKNULL(world, std::string{});
			return world->GetName();
		});
	}

	TList<const Instance> World_GetInstancesT(const World *world) noexcept
	{
		return TList<const Instance>::create(World_GetInstances(world));
	}

	const Terrain *World_GetTerrain(const World *world) noexcept
	{
		LIBSWBF2_LOG_INFO("World_GetTerrain({})", fmt::ptr(world));
		return catchall([&] {
			CHECKNULL(world, static_cast<const Terrain *>(nullptr));
			return world->GetTerrain();
		});
	}

	size_t World_GetTerrainName(const World *world, char *out_buffer, size_t out_buffer_len) noexcept
	{
		LIBSWBF2_LOG_INFO("World_GetTerrainName({}, {}, {})", fmt::ptr(world), fmt::ptr(out_buffer), out_buffer_len);
		return catchall([&] {
			CHECKNULL(world, static_cast<size_t>(0));
			std::string terrain_name = world->GetTerrainName();
			copy_c_str(terrain_name, out_buffer, out_buffer_len);
			return terrain_name.size();
		});
	}
}
