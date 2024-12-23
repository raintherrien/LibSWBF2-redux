#include "pch.h"
#include "Locl.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::Localization
{
	void Locl::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void Locl::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void Locl::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		// order of these two chunks seems to be fixed.
		// so no need for dynamic chunk checking.
		p_Name = ReadChild<STR<"NAME"_m>>(stream);
		p_Body = ReadChild<BODY>(stream);

		BaseChunk::EnsureEnd(stream);
	}

	std::string Locl::ToString() const
	{
		return fmt::format(
			"Language: {}\n"
			"Number of entries: {}",
			p_Name->m_Text,
			(uint32_t)p_Body->m_LocalizeEntries.size()
		);
	}
}
