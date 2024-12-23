#include "pch.h"
#include "coll.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"


namespace LibSWBF2::Chunks::LVL::coll
{
	void coll::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void coll::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void coll::ReadFromStream(FileReader& stream)
	{	   
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		p_ChunkName = ReadChild<STR<"NAME"_m>>(stream);

		if (stream.ReadChunkHeader(true) == "MASK"_h) {
			p_Mask = ReadChild<prim::MASK>(stream);
		} else {
			p_Mask = {};
		}

		p_NodeName = ReadChild<STR<"NODE"_m>>(stream);
		p_Info = ReadChild<INFO_coll>(stream);
		p_Verts = ReadChild<POSI>(stream);
		p_Tree = ReadChild<TREE>(stream);

		BaseChunk::EnsureEnd(stream);
	}

	std::string coll::ToString() const
	{
		return p_Tree -> ToString()  +
			   " Name: " + p_ChunkName -> ToString() +
				fmt::format(" Num verts: {}", p_Verts -> m_Verts.size());
	}
}
