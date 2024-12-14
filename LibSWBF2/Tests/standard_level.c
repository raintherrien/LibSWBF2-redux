#include "../API.h"
#include <stdio.h>

int main()
{
	struct Container *container = Container_Create();
	Container_AddLevel(container, "/home/rain/.local/share/Steam/steamapps/common/Star Wars Battlefront II Classic/GameData/data/_lvl_pc/geo/geo1.lvl");
	Container_Destroy(container);

	printf("Test complete\n");
	fflush(stdout);
	return 0;
}
