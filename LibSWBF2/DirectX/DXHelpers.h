// THIS IS NOT MEANT TO BE INCLUDED IN OTHER EXPOSDED HEADERS
#pragma once

#include "DirectX/D3D9FORMAT.h"
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2
{
	DXGI_FORMAT D3DToDXGI(D3DFORMAT d3dFormat);
	std::string D3DToString(D3DFORMAT d3dFormat);
	DXGI_FORMAT TextureFormatToDXGI(ETextureFormat format);
}
