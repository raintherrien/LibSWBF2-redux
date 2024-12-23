#include "pch.h"
#include "FACE.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::texture
{
	void FACE::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void FACE::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void FACE::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		while (ThereIsAnother(stream)) {
			m_LVLs.emplace_back(ReadChild<LVL_>(stream));
		}

		BaseChunk::EnsureEnd(stream);
	}
}
