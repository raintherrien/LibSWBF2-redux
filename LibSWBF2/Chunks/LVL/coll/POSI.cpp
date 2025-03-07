#include "pch.h"
#include "POSI.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::coll
{
	void POSI::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void POSI::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void POSI::ReadFromStream(FileReader& stream)
	{       
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        Vector3 tmp;
        while (ThereIsAnother(stream))
        {
            tmp.ReadFromStream(stream);
            m_Verts.push_back(tmp);
        }

		BaseChunk::EnsureEnd(stream);
	}

	std::string POSI::ToString() const
    {
	    std::string stringRep("");

        for (int i  = 0; i < m_Verts.size(); i++)
        {
            stringRep = stringRep + m_Verts[i].ToString() + "\n";
        }

        return stringRep;
    }
}
