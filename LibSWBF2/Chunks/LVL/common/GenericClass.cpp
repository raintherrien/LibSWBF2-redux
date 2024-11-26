#include "pch.h"
#include "GenericClass.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::common
{
	template<uint32_t Header>
	void GenericClass<Header>::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	template<uint32_t Header>
	void GenericClass<Header>::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	template<uint32_t Header>
	void GenericClass<Header>::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		GenericChunk<Header>::Check(stream);
		
        while (GenericChunk<Header>::ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
			if (next == "BASE"_h)
			{
				GenericChunk<Header>::READ_CHILD(stream, p_Base);
			}
			else if (next == "TYPE"_h)
			{
				GenericChunk<Header>::READ_CHILD(stream, p_Type);
			}
			else if (next == "PROP"_h)
			{
				PROP* prop;
				GenericChunk<Header>::READ_CHILD(stream, prop);
				m_Properties.push_back(prop);
			}
        }

		BaseChunk::EnsureEnd(stream);
	}

	template<uint32_t Header>
	std::string GenericClass<Header>::ToString() const
	{
		return fmt::format(
			"Base: {}\n\n"
			"Type: {}\n",
			p_Base->m_Text,
			p_Type->m_Text
		);
	}


	template struct LIBSWBF2_API GenericClass<0>;
	template struct LIBSWBF2_API GenericClass<"entc"_m>;
	template struct LIBSWBF2_API GenericClass<"ordc"_m>;
	template struct LIBSWBF2_API GenericClass<"wpnc"_m>;
	template struct LIBSWBF2_API GenericClass<"expc"_m>;
}
