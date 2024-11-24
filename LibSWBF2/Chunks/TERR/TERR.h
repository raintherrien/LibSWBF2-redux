#pragma once
#include "Chunks/HeaderNames.h"
#include "Types/Vector2.h"
#include <string>

namespace LibSWBF2
{
	class FileReader;
	class FileWriter;
}

namespace LibSWBF2::Chunks::LVL::terrain
{
	using LibSWBF2::Types::Vector2;

	// TODO: Move to Types into a dedicated header file
	struct LIBSWBF2_API TextureLayer
	{
		std::string DiffuseName;
		std::string DetailName;
	};

	// TODO: Move to Types into a dedicated header file
	struct LIBSWBF2_API WaterLayer
	{
		float_t WaterHeight;
		Vector2 UVAnimationVelocity;
		Vector2 UVAnimationRepeat;
		uint8_t RGBA[4]; // TODO: Introduce new Color32 type and use here
		std::string TextureName;
	};

	struct LIBSWBF2_API TERR
	{
		static TERR* Create();
		static void Destroy(TERR* terrain);

	protected:
		TERR() = default;
		~TERR() = default;

	public:
		void WriteToStream(FileWriter& stream);
		void ReadFromStream(FileReader& stream);
		bool WriteToFile(const std::string& Path);
		bool ReadFromFile(const std::string& Path);

#pragma region Header
		uint32_t FileVersion;
		uint16_t TerrainExtents[4];
		float_t TileRange[16];
		uint8_t TextureLayerMapping[16];
		float_t MapHeightMultiplier;
		float_t GridScale;
		uint32_t MapSize;
		uint32_t TerrainOptions;
		TextureLayer TextureLayers[16];
		WaterLayer WaterLayers[16];
		std::string RoadDecalTextureNames[16];
#pragma endregion Header
	};
}
