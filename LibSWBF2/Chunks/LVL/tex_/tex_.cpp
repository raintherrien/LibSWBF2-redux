#include "pch.h"
#include "tex_.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::texture
{
	void tex_::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void tex_::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void tex_::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		p_Name = ReadChild<STR<"NAME"_m>>(stream);
		p_Info = ReadChild<texture::INFO>(stream);

		while (ThereIsAnother(stream)) {
			m_FMTs.emplace_back(ReadChild<FMT_>(stream));
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string tex_::ToString() const
	{
		return fmt::format(
			"Name: {}\n\n"
			"Info: {}\n"
			"Number of FMTs: {}",
			p_Name->m_Text,
			p_Info->ToString(),
			m_FMTs.size()
		);
	}
}
