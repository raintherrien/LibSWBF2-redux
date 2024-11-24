#include "pch.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"

#include "float.DATA.h"


namespace LibSWBF2::Chunks::LVL::config
{
	void DATA_FLOAT::RefreshSize()
	{
		THROW("Not implemented!");
	}

	void DATA_FLOAT::WriteToStream(FileWriter& stream)
	{
		THROW("Not implemented!");
	}

	void DATA_FLOAT::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_NameHash = stream.ReadUInt32();
        
        uint8_t numFloats = stream.ReadByte();

        for (uint8_t i = 0; i < numFloats; i++)
        {
        	m_Floats.push_back(stream.ReadFloat());
        }


        BaseChunk::EnsureEnd(stream);
	}

	std::string DATA_FLOAT::ToString() const
	{
		std::string rep = fmt::format("Hash value: {}\nFloats: ", m_NameHash);
		for (uint8_t i = 0; i < m_Floats.size(); i++)
		{
			rep = rep + fmt::format("{} ", m_Floats[i]);
		}
		return rep;
	}
}
