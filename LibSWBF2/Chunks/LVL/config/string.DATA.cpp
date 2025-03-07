#include "pch.h"
#include "string.DATA.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"


namespace LibSWBF2::Chunks::LVL::config
{
	void DATA_STRING::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void DATA_STRING::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void DATA_STRING::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_Tag = stream.ReadUInt32();

        

        uint32_t numInts = (uint32_t) stream.ReadByte();
        stream.ReadString(numInts * 4); 

        uint32_t stringLength = stream.ReadUInt32();
        m_String = stream.ReadString(stringLength);   
            	

        BaseChunk::EnsureEnd(stream);
	}

	std::string DATA_STRING::ToString() const
	{
		if (m_String == "")
		{
			return "Global lighting section";
		}
		return m_String;
	}
}
