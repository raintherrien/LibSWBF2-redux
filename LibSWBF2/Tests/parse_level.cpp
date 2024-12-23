#include "../API.h"
#include <iostream>
#include <cstring>

using namespace LibSWBF2;

static struct Container_Owned *container = nullptr;

void import_texture(const LibSWBF2::Texture *texture)
{
	uint16_t width = 0;
	uint16_t height = 0;

	TList<uint8_t> buffer = Texture_GetDataT(texture, &width, &height);

	if (width == 0 || height == 0) {
		std::cerr << "Failed to load SWBF2 texture" << std::endl;
		return;
	}
}

void import_material(const LibSWBF2::Material *material)
{
	const LibSWBF2::Texture *texture = Material_GetTexture(material, 0);
	if (!texture) {
		std::cerr << "Failed to get albedo map texture for material" << std::endl;
		return;
	}

	std::string albedo_texture_name = Texture_GetNameS(texture);

	import_texture(texture); // Albedo

	// TODO: Other textures? Specular?
	// I'm assuming this matches the order of textures used by XSI as documented here:
	// https://sites.google.com/site/swbf2modtoolsdocumentation/misc_documentation
	// I am not confident LibSWBF2 correctly sets the BumpMap flag, so attempt to load
	// the second image of every material as normal map image.
	EMaterialFlags material_flags = Material_GetFlags(material);
	//if ((uint32_t)material_flags & (uint32_t)EMaterialFlags::BumpMap) {
		if (const LibSWBF2::Texture *texture = Material_GetTexture(material, 1)) {
			import_texture(texture); // Normal
		} else if ((uint32_t)material_flags & (uint32_t)EMaterialFlags::BumpMap) {
			std::cerr << "Failed to get normal map texture for material" << std::endl;
		}
	//}
}

void segments_to_mesh(const std::vector<const Segment *> &segments, const std::string &override_texture)
{
	for (size_t si = 0; si < segments.size(); ++ si) {
		const Segment *segment = segments[si];

		TList<uint16_t> index_buffer = Segment_GetIndexBufferT(segment);
		TList<const LibSWBF2::Vector3> vertex_buffer = Segment_GetVertexBufferT(segment);
		TList<const LibSWBF2::Vector2> tex_uv_buffer = Segment_GetUVBufferT(segment);
		TList<const LibSWBF2::Vector3> normal_buffer = Segment_GetNormalBufferT(segment);

		ETopology topology = Segment_GetTopology(segment);

		if (vertex_buffer.size() != normal_buffer.size() || normal_buffer.size() != tex_uv_buffer.size()) {
			std::cerr << "Skipping mesh with invalid vertex, normal, tex_uv count: "
			          << vertex_buffer.size() << " " << normal_buffer.size() << " " << tex_uv_buffer.size()
			          << std::endl;
			continue;
		}

		import_material(Segment_GetMaterial(segment));
	}
}

void populate_model(const std::string &model_name, const std::string &override_texture)
{
	std::cout << "Populating model " << model_name << std::endl;

	const Model *model = Container_FindModel(container, FNVHashString(model_name.c_str()));
	if (model == nullptr) {
		std::cerr << "Could not find model " << model_name << std::endl;
		return;
	}

	TList<const Bone> bones = Model_GetBonesT(model);
	for (size_t i = 0; i < bones.size(); ++ i) {
		const Bone *bone = bones.at(i);
		std::string bone_name = Bone_GetNameS(bone);
		std::string parent_name = Bone_GetParentNameS(bone);
		LibSWBF2::Vector3 pz = Bone_GetPosition(bone);
		LibSWBF2::Vector4 rz = Bone_GetRotation(bone);
	}

	TList<const Segment> model_segments = Model_GetSegmentsT(model);
	std::unordered_map<std::string, std::vector<const Segment *>> bone_segments;
	for (size_t i = 0; i < model_segments.size(); ++ i) {
		const Segment *segment = model_segments.at(i);
		std::string segment_bone = Segment_GetBoneNameS(segment);
		bone_segments[segment_bone].push_back(segment);
	}

	for (const auto &[bone_name, segments]: bone_segments) {
		segments_to_mesh(segments, override_texture);
	}

	TList<const CollisionPrimitive> collision_primitives = Model_GetCollisionPrimitivesT(model);
	for (size_t i = 0; i < collision_primitives.size(); ++ i) {
		const CollisionPrimitive *collision_primitive = collision_primitives.at(i);
		std::string parent_name = CollisionPrimitive_GetParentNameS(collision_primitive);
		LibSWBF2::Vector3 pz = CollisionPrimitive_GetPosition(collision_primitive);
		LibSWBF2::Vector4 rz = CollisionPrimitive_GetRotation(collision_primitive);
		ECollisionPrimitiveType pt = CollisionPrimitive_GetType(collision_primitive);
		switch (pt) {
			case ECollisionPrimitiveType::Cube: {
				float sx = 0.0f, sy = 0.0f, sz = 0.0f;
				CollisionPrimitive_GetCubeDims(collision_primitive, &sx, &sy, &sz);
				break;
			}
			case ECollisionPrimitiveType::Cylinder: {
				float sr = 0.0f, sh = 0.0f;
				CollisionPrimitive_GetCylinderDims(collision_primitive, &sr, &sh);
				break;
			}
			case ECollisionPrimitiveType::Sphere: {
				float sr = 0.0f;
				CollisionPrimitive_GetSphereRadius(collision_primitive, &sr);
				break;
			}
			default:
				std::cerr << "Skipping unsupported collision primitive type " << (int)pt << std::endl;
				break;
		}
	}

	// A model always has a collision mesh object, but it may be empty
	const CollisionMesh *collision_mesh = Model_GetCollisionMesh(model);
	TList<uint16_t> index_buffer = CollisionMesh_GetIndexBufferT(collision_mesh);
	if (index_buffer.size() > 0) {
		TList<LibSWBF2::Vector3> vertex_buffer = CollisionMesh_GetVertexBufferT(collision_mesh);
	}
}

static void import_entity_class(const std::string &entity_class_name)
{
	std::cout << "Importing EntityClass " << entity_class_name << std::endl;

	// Assert this instance is a type we understand
	const char *valid_base_classes[] = {
		"door",
		"animatedprop",
		"prop",
		"building",
		"destructablebuilding",
		"armedbuilding",
		"animatedbuilding",
		"commandpost"
	};
	bool is_valid_base_class = false;
	const EntityClass *entity_class = Container_FindEntityClass(container, FNVHashString(entity_class_name.c_str()));
	std::string base_class_name = "NONE";
	if (entity_class) {
		base_class_name = EntityClass_GetBaseNameS(entity_class);
		for (const char *valid_base_class : valid_base_classes) {
			if (strcmp(base_class_name.c_str(), valid_base_class) == 0) {
				is_valid_base_class = true;
				break;
			}
		}
	}
	if (!is_valid_base_class) {
		std::cerr << "Cannot import entity class " << entity_class_name << " of unknown base class " << base_class_name << std::endl;
		return;
	}

	TList<uint32_t> property_hashes = EntityClass_GetAllPropertyHashesT(entity_class);
	std::string next_attach_entity_class = "";
	for (size_t pi = 0; pi < property_hashes.size(); ++ pi) {
		uint32_t property_hash = *property_hashes.at(pi);
		std::string property_value = EntityClass_GetPropertyValueS(entity_class, property_hash);
		switch (property_hash) {
			case 1204317002: { // GeometryName
				std::cout << "Attaching model " << property_value << " to " << entity_class_name << std::endl;
				// Determine our texture, which is a separate property
				std::string override_texture = "";
				for (size_t i = 0; i < property_hashes.size(); ++ i) {
					if (*property_hashes.at(i) == 165377196) {
						override_texture = EntityClass_GetPropertyValueS(entity_class, property_hash);
						break;
					}
				}
				populate_model(property_value, override_texture);
				break;
			}
			case 2849035403: // AttachODF
				next_attach_entity_class = property_value;
				break;
			case 1005041674: // AttachToHardpoint
				if (next_attach_entity_class.length() > 0) {
					std::cout << "Attaching " << next_attach_entity_class << " to hardpoint " << property_value << std::endl;
					import_entity_class(next_attach_entity_class);
					next_attach_entity_class = "";
				} else {
					std::cerr << "Skipping AttachToHardpoint " << property_value << " not preceeded by an AttachODF" << std::endl;
				}
				break;
			case 2555738718: // AnimationName
			case 3779456605: // Animation
				std::cerr << "Skipping animation " << property_value << std::endl;
				break;
			case 1576910975: // SoldierCollision
			case 4213956359: // OrdnanceCollision
				// Silently ignore collision masking
				break;
			case 165377196: // OverrideTexture
				// Silently ignore, only relevant in GeometryName
				break;
			case 2714356677: // FoleyFXGroups
				break;
			default:
				std::cerr << "Skipping unknown property " << property_hash << " (hash) = " << property_value << std::endl;
				break;
		}
	}
	if (next_attach_entity_class != "") {
		std::cerr << "Skipping AttachODF property " << next_attach_entity_class << " not followed by an AttachToHardpoint property" << std::endl;
	}
}

static void import_terrain(const struct World *world)
{
	std::cout << "Importing terrain" << std::endl;
	// XXX
}

static void import_skydome(const struct World *world)
{
	std::cout << "Importing skydome" << std::endl;
	// XXX
}

static void import_world(const struct World *world)
{
	std::string world_name = World_GetNameS(world);
	std::cout << "Importing world " << world_name << std::endl;

	TList<const Instance> instances = World_GetInstancesT(world);
	for (size_t i = 0; i < instances.size(); ++ i) {
		continue;
		const struct Instance *instance = instances.at(i);
		struct Vector3 p = Instance_GetPosition(instance);
		struct Vector4 r = Instance_GetRotation(instance);
		std::string instance_name = Instance_GetNameS(instance);
		std::string entity_class_name = Instance_GetEntityClassNameS(instance);
		std::cout << "Importing instance " << instance_name
		          << " of entity class " << entity_class_name
		          << " at position (" << p.m_X << "," << p.m_Y << "," << p.m_Z << ")"
		          << std::endl;
		import_entity_class(entity_class_name);
	}

	import_terrain(world);
	import_skydome(world);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "Usage: parse_level {path-to-lvl}" << std::endl;
		return 0;
	}

	container = Container_Create();
	struct Level_Owned *level = Container_AddLevel(container, argv[1]);
	if (level) {
		if (Level_IsWorldLevel(level)) {
			TList<const World> worlds = Level_GetWorldsT(level);
			for (size_t i = 0; i < worlds.size(); ++ i) {
				const struct World *world = worlds.at(i);
				import_world(world);
			}
		} else {
			std::cerr << "Level is not a world level" << std::endl;
		}
		Level_Destroy(level);
	} else {
		std::cerr << "Failed to load level" << std::endl;
	}
	Container_Destroy(container);
	std::cout << "Test complete" << std::endl;
	return 0;
}

