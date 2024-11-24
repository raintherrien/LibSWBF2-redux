#pragma once
#include "Chunks/GenericChunk.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::sound
{
	struct Stream;

	struct LIBSWBF2_API StreamList : public GenericChunk<"StreamList"_fnv>
	{
		std::vector<Stream *> m_Streams;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
