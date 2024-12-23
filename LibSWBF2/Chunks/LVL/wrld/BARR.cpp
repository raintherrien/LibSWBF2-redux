#include "pch.h"
#include "BARR.h"

#include "XFRM.h"
#include "SIZE.h"

#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{

    void BARR_FLAG::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BARR_FLAG::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BARR_FLAG::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);
        m_Flag = stream.ReadUInt32();
        BaseChunk::EnsureEnd(stream);
    }

    std::string BARR_FLAG::ToString() const
    {
        return fmt::format("{0}", m_Flag);
    }




    void BARR_INFO::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BARR_INFO::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BARR_INFO::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "NAME"_h)
            {
                p_Name = ReadChild<STR<"NAME"_m>>(stream);
            }
            else if (next == "XFRM"_h)
            {
                p_Transform = ReadChild<XFRM>(stream);
            }
            else if (next == "SIZE"_h)
            {
                p_Size = ReadChild<SIZE>(stream);
            }
            else if (next == "FLAG"_h)
            {
                p_Flag = ReadChild<BARR_FLAG>(stream);
            }
            else 
            {
                (void) ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }


    std::string BARR_INFO::ToString() const
    {
        return p_Name -> ToString();
    }





    void BARR::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BARR::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BARR::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "INFO"_h)
            {
                p_Info = ReadChild<BARR_INFO>(stream);
            }
            else 
            {
                (void) ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }


    std::string BARR::ToString() const
    {
        return p_Info == nullptr ? "" : p_Info -> ToString();
    }
}
