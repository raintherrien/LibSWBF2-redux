#include "pch.h"
#include "prim.DATA.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"


namespace LibSWBF2::Chunks::LVL::prim
{
	void DATA_PRIM::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void DATA_PRIM::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void DATA_PRIM::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
        Check(stream);

        uint32_t rawUInt = stream.ReadUInt32();

        m_PrimitiveType = (ECollisionPrimitiveType) (rawUInt == 0 ? 0x1 : rawUInt);

        m_Field1 = stream.ReadFloat();
        m_Field2 = stream.ReadFloat();
        m_Field3 = stream.ReadFloat();

        BaseChunk::EnsureEnd(stream);
	}

	std::string DATA_PRIM::ToString() const
	{
		switch (m_PrimitiveType)
		{
			case ECollisionPrimitiveType::Sphere:
				return fmt::format("Type: Sphere, Radius: {}", 
									m_Field1);
			case ECollisionPrimitiveType::Cylinder:
				return fmt::format("Type: Cylinder, Radius: {}, Height: {}",
									m_Field1, m_Field2);
			case ECollisionPrimitiveType::Cube:
				return fmt::format("Type: Cube, x: {}, y: {}, z: {}",
									m_Field1, m_Field2, m_Field3);
			case ECollisionPrimitiveType::Empty:
				return "Empty";
				
			default:
				std::string unknown = CollisionPrimitiveTypeToString(m_PrimitiveType);
				return fmt::format("{}, field1: {}, field2: {}, field3: {}",
									unknown, m_Field1, m_Field2, m_Field3);
		}
	}
}
