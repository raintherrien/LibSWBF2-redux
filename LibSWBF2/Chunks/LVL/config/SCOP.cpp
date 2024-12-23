#include "pch.h"
#include "Logging/Logger.h"
#include "DirectX/DXHelpers.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "SCOP.h"
#include <algorithm>
#include <cstring>
#include <iostream>


namespace LibSWBF2::Chunks::LVL::config
{
    void SCOP::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void SCOP::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void SCOP::ReadFromStream(FileReader& stream)
    { 
        BaseChunk::ReadFromStream(stream);
        Check(stream);
        
        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);

            if (next == "DATA"_h)
            {
                ReadChild<DATA_CONFIG>(stream);
            }
            else if (next == "SCOP"_h)
            {
                ReadChild<SCOP>(stream);
            }
            else 
            {
                ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);        
    }
}
