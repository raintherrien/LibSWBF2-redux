#include "../API.h"
#include <stdio.h>

void import_world(const struct World *world)
{
	char world_name[64];
	World_GetName(world, world_name, 64);
	printf("Importing world %s\n", world_name);

	struct CList instances = World_GetInstances(world);
	for (size_t i = 0; i < instances.size; ++ i) {
		const struct Instance *instance = CList_get(&instances, i);
		struct Vector3 p = Instance_GetPosition(instance);
		struct Vector4 r = Instance_GetRotation(instance);
		char instance_name[64];
		char entity_class_name[64];
		Instance_GetName(instance, instance_name, 64),
		Instance_GetEntityClassName(instance, entity_class_name, 64);
		printf("Importing instance %s of entity class %s at position (%f,%f,%f) with rotation (%f,%f,%f,%f)\n",
			instance_name,
			entity_class_name,
			p.m_X, p.m_Y, p.m_Z,
			r.m_X, r.m_Y, r.m_Z, r.m_W);
		// XXX import_entity_class(entity_class_name);
	}
	CList_free(&instances);

	// XXX Import terrain

	// XXX Import skydome
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: parse_level {path-to-lvl}\n");
		return 0;
	}

	struct Container *container = Container_Create();
	const struct Level *level = Container_AddLevel(container, argv[1]);
	if (level) {
		if (Level_IsWorldLevel(level)) {
			struct CList worlds = Level_GetWorlds(level);
			for (size_t i = 0; i < worlds.size; ++ i) {
				const struct World *world = CList_get(&worlds, i);
				import_world(world);
			}
			CList_free(&worlds);
		} else {
			fprintf(stderr, "Level is not a world level\n");
		}
	} else {
		fprintf(stderr, "Failed to load level\n");
	}

	Container_Destroy(container);
	printf("Test complete\n");
	fflush(stdout);
	return 0;
}

