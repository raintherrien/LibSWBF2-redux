#include "pch.h"
#include "Hint.h"

#include "XFRM.h"

#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::wrld
{

    void Hint_TYPE::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void Hint_TYPE::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void Hint_TYPE::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);
        m_Type = stream.ReadUInt16();
        BaseChunk::EnsureEnd(stream);
    }

    std::string Hint_TYPE::ToString() const
    {
        return fmt::format("{0}", m_Type);
    }




    void Hint_INFO::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void Hint_INFO::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void Hint_INFO::ReadFromStream(FileReader& stream)
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
            else if (next == "TYPE"_h)
            {
                p_Type = ReadChild<Hint_TYPE>(stream);
            }
            else 
            {
                (void) ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }


    std::string Hint_INFO::ToString() const
    {
        return p_Name -> ToString();
    }





    void Hint::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void Hint::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void Hint::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        while (ThereIsAnother(stream))
        {
            ChunkHeader next = stream.ReadChunkHeader(true);
            if (next == "INFO"_h)
            {
                p_Info = ReadChild<Hint_INFO>(stream);
            }
            else if (next == "PROP"_h)
            {
                m_Properties.emplace_back(ReadChild<PROP>(stream));
            }
            else 
            {
                (void) ReadChild<GenericChunk>(stream);
            }
        }

        BaseChunk::EnsureEnd(stream);
    }


    std::string Hint::ToString() const
    {
        return p_Info == nullptr ? "" : p_Info -> ToString();
    }
}
