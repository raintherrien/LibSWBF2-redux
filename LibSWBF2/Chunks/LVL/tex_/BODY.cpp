#include "pch.h"
#include "BODY.h"
#include "FMT_.h"
#include "Logging/Logger.h"
#include "DirectX/DXHelpers.h"
#include "DirectX/DXTexCrossPlat.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include <algorithm>
#include <cstring>


namespace LibSWBF2::Chunks::LVL::LVL_texture
{
    using LVL::texture::FMT_;

    void BODY::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BODY::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void BODY::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        const LVL_* lvl = dynamic_cast<const LVL_*>(GetParent());

        //                                                  FACE         FMT_
        const FMT_* fmt = dynamic_cast<const FMT_*>(lvl->GetParent()->GetParent());
        if (fmt == nullptr)
        {
            LIBSWBF2_LOG_ERROR("Could not grab FMT parent!");
            BaseChunk::EnsureEnd(stream);
            return;
        }

        if (p_Image != nullptr)
        {
            delete p_Image;
            p_Image = nullptr;
        }

        size_t width = fmt->p_Info->m_Width;
        size_t height = fmt->p_Info->m_Height;
        // mip levels start at 0
        // divide resolution by 2 for each increasing mip level
        size_t div = (size_t)std::pow(2, lvl->p_Info->m_MipLevel);

        // don't go below 2x2 pixels, DirectX will crash otherwise
        // in e.g. geo1.lvl there's a case with a 512x256 image with
        // a mip level up to 9 (dafuq)
        width = std::max(width / div, (size_t)2);
        height = std::max(height / div, (size_t)2);

        size_t dataSize = GetDataSize();
        D3DFORMAT d3dFormat = fmt->p_Info->m_Format;

        // it seems like sometimes (in core.lvl) the specified image size
        // is far smaller than the actual BODY data size... So just do a quick
        // sanity check here before allocating and performing a memcpy
        if (dataSize > width * height * 4)
        {
            stream.SkipBytes(dataSize);
            LIBSWBF2_LOG_WARN("Suspicious image size: {}! Skipping image!", dataSize);
            BaseChunk::EnsureEnd(stream);
            return;
        }

        uint8_t* imageBufferPtr;
        p_Image = new DXTexCrossPlat::CrossPlatImage(width, height, d3dFormat, dataSize);
        imageBufferPtr = p_Image -> GetPixelsPtr();

        if (imageBufferPtr == nullptr || !stream.ReadBytes(imageBufferPtr, dataSize))
        {
            LIBSWBF2_LOG_ERROR("Reading image data of size '{}' failed!", dataSize);
            BaseChunk::EnsureEnd(stream);
            return;
        }

        BaseChunk::EnsureEnd(stream);       
    }


    bool BODY::GetImageData(ETextureFormat format, uint16_t& width, uint16_t& height, const uint8_t*& data) const
    {
        if (p_Image == nullptr)
        {
            LIBSWBF2_LOG_WARN("No image!");
            width = 0;
            height = 0;
            data = nullptr;
            return false;
        }

        if (!p_Image -> IsConvertibleTo(D3DFMT_R8G8B8A8))
        {
            data = nullptr;
            return false;
        }

        p_Image -> ConvertTo(D3DFMT_R8G8B8A8);

        width = p_Image->width;
        height = p_Image->height;
        data = p_Image -> GetPixelsPtr();

        return true;
    }

    BODY::~BODY()
    {
        delete p_Image;
    }

}
