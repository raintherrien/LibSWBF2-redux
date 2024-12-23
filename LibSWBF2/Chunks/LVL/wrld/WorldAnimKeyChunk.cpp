#include "pch.h"
#include "WorldAnimKeyChunk.h"
#include "InternalHelpers.h"
#include "FileReader.h"


namespace LibSWBF2::Chunks::LVL::wrld
{
	template<uint32_t Header>
	void WorldAnimKeyChunk<Header>::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	template<uint32_t Header>
	void WorldAnimKeyChunk<Header>::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	template<uint32_t Header>
	void WorldAnimKeyChunk<Header>::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);
			
    	m_Key.ReadFromStream(stream);

		BaseChunk::EnsureEnd(stream);
	}

	template<uint32_t Header>
	std::string WorldAnimKeyChunk<Header>::ToString() const
	{
		return m_Key.ToString();
	}


	template struct LIBSWBF2_API WorldAnimKeyChunk<0>;
	template struct LIBSWBF2_API WorldAnimKeyChunk<"POSK"_m>;
	template struct LIBSWBF2_API WorldAnimKeyChunk<"ROTK"_m>;
}
