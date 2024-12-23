#include "pch.h"
#include "ConfigChunk.h"
#include "InternalHelpers.h"
#include "FileReader.h"

#include "SCOP.h"
#include "DATA.h"

namespace LibSWBF2::Chunks::LVL::config
{
	template<uint32_t Header>
	void ConfigChunk<Header>::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	template<uint32_t Header>
	void ConfigChunk<Header>::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	template<uint32_t Header>
	void ConfigChunk<Header>::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);
		
		
        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
			if (next == "NAME"_h)
			{
				p_Hash = ReadChild<config_NAME>(stream);
			}
			else if (next == "DATA"_h)
			{
				(void) ReadChild<DATA_CONFIG>(stream);
			}
			else if (next == "SCOP"_h)
			{
				(void) ReadChild<SCOP>(stream);
			}
			else 
			{
				LIBSWBF2_LOG_WARN("Irregular config chunk child found ({0:x})!", Header);
				BaseChunk::EnsureEnd(stream);
			}
        }

		BaseChunk::EnsureEnd(stream);
	}

	template<uint32_t Header>
	std::string ConfigChunk<Header>::ToString() const
	{
		if (p_Hash)
		{
			std::string name;
			return fmt::format(
				"Hash: 0x{0:x}\n"
				"Lookup Name: {1}",
				p_Hash -> m_Name,
				FNV::Lookup(p_Hash->m_Name, name) ? name : ""
			).c_str();
		}
		return "No Name";
	}


	template struct LIBSWBF2_API ConfigChunk<0>;
	template struct LIBSWBF2_API ConfigChunk<"lght"_m>;
	template struct LIBSWBF2_API ConfigChunk<"fx__"_m>;
	template struct LIBSWBF2_API ConfigChunk<"sky_"_m>;
	template struct LIBSWBF2_API ConfigChunk<"bnd_"_m>;
	template struct LIBSWBF2_API ConfigChunk<"prp_"_m>;
	template struct LIBSWBF2_API ConfigChunk<"path"_m>;
	template struct LIBSWBF2_API ConfigChunk<"comb"_m>;
	template struct LIBSWBF2_API ConfigChunk<"snd_"_m>;
	template struct LIBSWBF2_API ConfigChunk<"mus_"_m>;
	template struct LIBSWBF2_API ConfigChunk<"ffx_"_m>;
	template struct LIBSWBF2_API ConfigChunk<"tsr_"_m>;
	template struct LIBSWBF2_API ConfigChunk<"hud_"_m>;
}
