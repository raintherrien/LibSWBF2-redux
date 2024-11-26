#include "pch.h"
#include "LTEX.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "tern.h"

namespace LibSWBF2::Chunks::LVL::terrain
{
    void LTEX::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void LTEX::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void LTEX::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        tern* parent = dynamic_cast<tern*>(GetParent());
        if (parent == nullptr)
        {
            LIBSWBF2_LOG_ERROR("Parent of LTEX is not a tern chunk!");
            return;
        }

        INFO* info = parent->p_Info;
        if (info == nullptr)
        {
            LIBSWBF2_LOG_ERROR("Cannot read texture layers, terrain has no Info chunk!");
            return;
        }

        m_LayerTextures.clear();
        for (uint16_t i = 0; i < info->m_TextureCount; ++i)
        {
            m_LayerTextures.push_back(stream.ReadString());
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string LTEX::ToString() const
    {
        std::string result;
        for (size_t i = 0; i < m_LayerTextures.size(); ++i)
        {
            result += fmt::format("Layer {}: {}\n", i, m_LayerTextures[i]);
        }
        return result;
    }
}
