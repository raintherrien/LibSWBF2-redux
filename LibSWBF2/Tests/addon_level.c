#include "../API.h"

int main()
{
	struct Container *container = Container_Create();
	Container_AddLevel(container, "/home/rain/.local/share/Steam/steamapps/common/Star Wars Battlefront II Classic/GameData/AddOn/DLC_XBOX/data/_LVL_PC/YAV/yav2.lvl");
	Container_Destroy(container);
	return 0;
}
