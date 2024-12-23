#include "pch.h"
#include "prim.h"

#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"



namespace LibSWBF2::Chunks::LVL::prim
{
	void prim::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void prim::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void prim::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		p_InfoChunk = ReadChild<INFO_prim>(stream);
		ChunkHeader nextHeader;

		for (int i = 0; i < p_InfoChunk -> m_NumPrimitives; i++)
		{
			m_PrimitiveNAMEs.emplace_back(ReadChild<STR<"NAME"_m>>(stream));

			if (stream.ReadChunkHeader(true) == "MASK"_h) {
				m_PrimitiveMASKs.emplace_back(ReadChild<MASK>(stream));
			} else {
				m_PrimitiveMASKs.emplace_back();
			}

			m_PrimitivePRNTs.emplace_back(ReadChild<STR<"PRNT"_m>>(stream));

			m_PrimitiveXFRMs.emplace_back(ReadChild<wrld::XFRM>(stream));

			m_PrimitiveDATAs.emplace_back(ReadChild<DATA_PRIM>(stream));
		}

		BaseChunk::EnsureEnd(stream);
	}


	std::string prim::ToString() const
	{
		return p_InfoChunk -> ToString();
	}
}
