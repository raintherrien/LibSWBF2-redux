#include "pch.h"
#include "PTCH.VBUF.h"
#include "PTCH.INFO.h"
#include "PTCH.h"
#include "InternalHelpers.h"
#include "FileReader.h"


namespace LibSWBF2::Chunks::LVL::terrain
{
    void VBUF::RefreshSize()
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void VBUF::WriteToStream(FileWriter& stream)
    {
        LIBSWBF2_THROW("Not implemented!");
    }

    void VBUF::ReadFromStream(FileReader& stream)
    {
        BaseChunk::ReadFromStream(stream);
        Check(stream);

        m_ElementCount = stream.ReadUInt32();
        m_ElementSize = stream.ReadUInt32();
        m_BufferType = (ETerrainBufferType)stream.ReadUInt32();

        //LIBSWBF2_LOG_WARN("On vbuf: numElements: {}, elementSize: {}", m_ElementCount, m_ElementSize);

        if (m_BufferType == ETerrainBufferType::Geometry)
        {
            //if (m_ElementCount != 81)
            //{
            //    LIBSWBF2_LOG_WARN("Invalid geometry VBUF terrain chunk found with ElementCount: {}", m_ElementCount);
            //}
            //else if (m_ElementSize != 28 && m_ElementSize != 16)
            //{
            //    LIBSWBF2_LOG_WARN("Invalid geometry VBUF terrain chunk found with ElementSize: {}", m_ElementSize);
            //}
            //else
            //{
                for (uint32_t i = 0; i < m_ElementCount; ++i)
                {
                    m_TerrainBuffer.emplace_back().ReadFromStream(stream);
                }
            //}
        }
        else if (m_BufferType == ETerrainBufferType::Texture)
        {
            //I'm so far as baffled as you as to why these are so...
            //The other data stored in each element isn't garbage 
            //or all default, it is mostly patterned.  Exact 
            //pattern/meaning still unknown.
            const static int KNOWN_STRENGTH_OFFSETS[] = {15, 11, 6};

            PTCH *parentPatch = dynamic_cast<PTCH*>(GetParent());
	    std::shared_ptr<PTCH_INFO> patchInfo = parentPatch -> p_PatchInfo;

	    std::vector<uint32_t>& slotsList = patchInfo -> m_TextureSlotsUsed;
            uint32_t numSlotsUsed = (uint32_t)slotsList.size();
            
            //Temp buffer for storing each raw (usually 16 byte long)
            //VBUF element
            uint8_t *elementBuffer = new uint8_t[m_ElementSize]();

            for (uint32_t i = 0; i < m_ElementCount * numSlotsUsed; i += numSlotsUsed)
            {
                stream.ReadBytes(elementBuffer, m_ElementSize);

                for (uint32_t j = 0; j < numSlotsUsed; j++)
                {                 
                    //Don't know how > 3 strengths are stored per VBUF element just yet
                    uint32_t newElement = j < 3 ? elementBuffer[KNOWN_STRENGTH_OFFSETS[j]] : 0;
                    m_BlendMapData.push_back(newElement);
                }
            }

            delete[] elementBuffer;
        }
        else
        {
            LIBSWBF2_LOG_INFO("Skip yet unsupported Terrain Buffer Type: {}", TerrainBufferTypeToString(m_BufferType));
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string VBUF::ToString() const
    {
        std::string result = fmt::format(
            "Element Count: {}\n"
            "Element Size: {}\n"
            "Buffer Type: {}\n"
            "\n"
            "Buffer: \n\n",
            m_ElementCount,
            m_ElementSize,
            TerrainBufferTypeToString(m_BufferType)
        );

        for (uint32_t i = 0; i < m_TerrainBuffer.size(); ++i)
        {
            result += m_TerrainBuffer[i].ToString();
            result += "\n\n";
        }

        return result;
    }
}
