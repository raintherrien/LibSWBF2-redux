#include "pch.h"
#include "FMT_.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::texture
{
	void FMT_::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void FMT_::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void FMT_::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		p_Info = ReadChild<FMT::INFO>(stream);
		p_Face = ReadChild<FACE>(stream);

		BaseChunk::EnsureEnd(stream);
	}
}
