#include "pch.h"
#include "anim.h"
//#include "anim.INFO.h"
#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"
//#include "Chunks/LVL/wrld/WorldAnimKeyChunk.h"


namespace LibSWBF2::Chunks::LVL::wrld
{
    void anim::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anim::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void anim::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "INFO"_h)
            {
		p_Info = ReadChild<anim_INFO>(stream);
            }
            else if (next == "POSK"_h)
            {
		m_PositionKeys.emplace_back(ReadChild<POSK>(stream));
            }
            else if (next == "ROTK"_h)
            {
		m_RotationKeys.emplace_back(ReadChild<ROTK>(stream));
            }
            else
            {
                ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }
}
