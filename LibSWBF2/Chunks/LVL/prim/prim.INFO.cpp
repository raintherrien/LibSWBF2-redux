#include "pch.h"
#include "prim.INFO.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"


namespace LibSWBF2::Chunks::LVL::prim
{
	void INFO_prim::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void INFO_prim::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void INFO_prim::ReadFromStream(FileReader& stream)
	{       
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_ModelName  = stream.ReadString();
        m_NumPrimitives = stream.ReadUInt16();

		BaseChunk::EnsureEnd(stream);
	}

	std::string INFO_prim::ToString() const 
    {
    	return fmt::format("String member: {}\nNum primitives {}",
    		   m_ModelName, m_NumPrimitives);
    }
}
