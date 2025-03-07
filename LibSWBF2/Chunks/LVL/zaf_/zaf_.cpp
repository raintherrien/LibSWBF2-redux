#include "pch.h"
#include "zaf_.h"
#include "BIN_.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Exceptions.h"

namespace LibSWBF2::Chunks::LVL::animation
{
	void zaf_::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void zaf_::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void zaf_::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		p_Name = ReadChild<STR<"NAME"_m>>(stream);

		//try 
		//{
			p_Bin = ReadChild<ZAF_BIN_>(stream);
		//}
		//catch (const LibSWBF2Exception &le)
		//{
		//	LIBSWBF2_LOG_ERROR("{0}", le.what());
		//	p_Bin = nullptr;
		//}

		BaseChunk::EnsureEnd(stream);
	}

	std::string zaf_::ToString() const
	{
		return fmt::format(
			"Animation skeleton of name: {}\n\n",
			p_Name->m_Text
		);
	}
}
