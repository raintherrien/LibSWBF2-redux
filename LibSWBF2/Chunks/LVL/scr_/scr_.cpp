#include "pch.h"
#include "scr_.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::script
{
	void scr_::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void scr_::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void scr_::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		p_Name = ReadChild<STR<"NAME"_m>>(stream);
		p_Info = ReadChild<INFO>(stream);
		p_Body = ReadChild<RawData<"BODY"_m>>(stream);

		BaseChunk::EnsureEnd(stream);
	}

	std::string scr_::ToString() const
	{
		return fmt::format(
			"Name: {}\n\n"
			"Info: {}\n",
			p_Name->m_Text,
			p_Info->ToString()
		);
	}
}
