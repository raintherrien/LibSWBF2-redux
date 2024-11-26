#include "pch.h"
#include "zaa_.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Exceptions.h"

namespace LibSWBF2::Chunks::LVL::animation
{
	void zaa_::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void zaa_::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void zaa_::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		READ_CHILD(stream, p_Name);

		try 
		{
			READ_CHILD(stream, p_Bin);
		}
		catch (const LibSWBF2Exception &le)
		{

			LIBSWBF2_LOG_ERROR("{}", le.what());
			p_Bin = nullptr;
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string zaa_::ToString() const
	{
		return fmt::format(
			"Name: {}\n\n",
			p_Name->m_Text
		);
	}
}
