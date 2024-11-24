#pragma once

#include "Chunks/GenericChunk.h"
#include "Types/Planning.h"
#include <string>
#include <vector>

using LibSWBF2::Types::Hub;
using LibSWBF2::Types::Connection;


namespace LibSWBF2::Chunks::plnp
{
	struct LIBSWBF2_API plnp : public GenericChunk<"plnp"_m>
	{
		std::vector<uint8_t> m_NumConnectionsInEachGroup;
		std::vector<FNVHash> m_ConnectionHashes;

		void ReadFromStream(FileReader& stream) override;
		std::string ToString() const override;
	};
}
