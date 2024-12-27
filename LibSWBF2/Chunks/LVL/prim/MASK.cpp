#include "pch.h"
#include "MASK.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"



namespace LibSWBF2::Chunks::LVL::prim
{
    void MASK::RefreshSize()
    {
    	LIBSWBF2_THROW("Not implemented!");
    }

    void MASK::WriteToStream(FileWriter& stream)
    {
    	LIBSWBF2_THROW("Not implemented!");
    }

    void MASK::ReadFromStream(FileReader& stream)
    {       
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_MaskFlags = (ECollisionMaskFlags) stream.ReadUInt32();

        BaseChunk::EnsureEnd(stream);
    }

    std::string MASK::ToString() const
    {
        return CollisionMaskTypeToString(m_MaskFlags);
    }
}
