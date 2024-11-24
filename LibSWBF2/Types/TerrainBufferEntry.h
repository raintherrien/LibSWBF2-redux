#pragma once
#include "Vector3.h"
#include "Color4u8.h"

namespace LibSWBF2
{
	class FileReader;
	class FileWriter;
}

namespace LibSWBF2::Types
{
	struct LIBSWBF2_API TerrainBufferEntry
	{
		void WriteToStream(FileWriter& stream);
		void ReadFromStream(FileReader& stream);

		std::string ToString() const;

		Vector3 m_Position;
		Vector3 m_Normal;
		Color4u8 m_Color;
	};
}
