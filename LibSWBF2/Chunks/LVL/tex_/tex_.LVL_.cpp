#include "pch.h"
#include "tex_.LVL_.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::LVL_texture
{
	void LVL_::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void LVL_::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void LVL_::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		p_Info = ReadChild<INFO>(stream);
		p_Body = ReadChild<BODY>(stream);

		BaseChunk::EnsureEnd(stream);
	}
}
