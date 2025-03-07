#include "pch.h"
#include "modl.VBUF.h"
#include "LVL.modl.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Logging/Logger.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include <limits>

#undef min
#undef max

namespace LibSWBF2::Chunks::LVL::modl
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

        m_Count = stream.ReadUInt32();
        m_Stride = stream.ReadUInt32();
        m_Flags = (EVBUFFlags)stream.ReadUInt32();

        modl* model = dynamic_cast<modl*>(GetParent()->GetParent());
        if (model == nullptr)
        {
            // should never happen
            LIBSWBF2_LOG_ERROR("Parent modl is NULL!");
            BaseChunk::EnsureEnd(stream);
            return;
        }

        for (uint32_t i = 0; i < m_Count; ++i)
        {
            if ((m_Flags & EVBUFFlags::Position) != 0)
            {
                if ((m_Flags & EVBUFFlags::PositionCompressed) != 0)
                {
                    glm::vec3 low = ToGLM(model->p_Info->m_VertexBox[0]);
                    glm::vec3 mul = (ToGLM(model->p_Info->m_VertexBox[1]) - ToGLM(model->p_Info->m_VertexBox[0]));

                    int16_t data[4];
                    data[0] = stream.ReadInt16();
                    data[1] = stream.ReadInt16();
                    data[2] = stream.ReadInt16();
                    data[3] = stream.ReadInt16();
                    glm::vec3 c(data[0], data[1], data[2]);

                    constexpr float i16min = std::numeric_limits<int16_t>::min();
                    constexpr float i16max = std::numeric_limits<int16_t>::max();

                    m_Positions.push_back(ToLib(low + (c - i16min) * mul / (i16max - i16min)));
                }
                else
                {
                    m_Positions.emplace_back().ReadFromStream(stream);
                }
            }

            if ((m_Flags & EVBUFFlags::BlendWeight) != 0)
            {
                if ((m_Flags & EVBUFFlags::BlendWeightCompressed) != 0)
                {
                    int8_t data[4];
                    data[0] = stream.ReadByte();
                    data[1] = stream.ReadByte();
                    data[2] = stream.ReadByte();
                    data[3] = stream.ReadByte();

                    float_t one = (float) data[1];
                    float_t two = (float) data[2];
                    m_Weights.push_back({ two, one, 1.0f - two - one});
                }
                else
                {
                    float_t x = stream.ReadFloat();
                    float_t y = stream.ReadFloat();
                    m_Weights.push_back({ x, y, 1.0f - x - y});
                }
            }

            if ((m_Flags & EVBUFFlags::Unknown1) != 0)
            {

                uint32_t inds = stream.ReadUInt32();

                uint8_t x = (uint8_t) (inds & 0xffu);
                uint8_t y = (uint8_t) ((inds >> 8u) & 0xffu);
                uint8_t z = (uint8_t) ((inds >> 16u) & 0xffu);


                m_BoneIndicies.push_back({x, y, z});


                /*
                uint8_t data[4];
                data[0] = stream.ReadByte();
                data[1] = stream.ReadByte();
                data[2] = stream.ReadByte();
                data[3] = stream.ReadByte();

                if ((m_Flags & EVBUFFlags::PositionCompressed) != 0)
                {
                    m_Bones.emplace_back().ReadFromStream(stream);
                    LIBSWBF2_LOG_WARN("Weight?: {}", stream.ReadFloat());

                    uint16_t data2[2];
                    data2[0] = *(uint16_t*)&data[0];
                    data2[1] = *(uint16_t*)&data[2];
                    uint32_t data3 = *(uint32_t*)&data[0];
                    LIBSWBF2_LOG_WARN("[{}] = {}-{}-{}-{} / {} - {} / {}", i, data[0], data[1], data[2], data[3], data2[0], data2[1], data3);

                    std::string hash = lookup_fnv_hash(data3);
                    LIBSWBF2_LOG_WARN("Hash: {}", hash);
                }
                */
            }

            if ((m_Flags & EVBUFFlags::Normal) != 0)
            {
                if ((m_Flags & EVBUFFlags::NormalCompressed) != 0)
                {
                    int8_t data[4];
                    data[0] = stream.ReadByte();
                    data[1] = stream.ReadByte();
                    data[2] = stream.ReadByte();
                    data[3] = stream.ReadByte();
                    glm::vec3 normal((float_t)data[0], (float_t)data[1], (float_t)data[2]);
                    normal = (normal * 2.0f) - 1.0f;
                    m_Normals.push_back(ToLib(normal));
                }
                else
                {
                    m_Normals.emplace_back().ReadFromStream(stream);
                }
            }

            if ((m_Flags & EVBUFFlags::Tangents) != 0)
            {
                if ((m_Flags & EVBUFFlags::NormalCompressed) != 0)
                {
                    int8_t data[4];
                    data[0] = stream.ReadByte();
                    data[1] = stream.ReadByte();
                    data[2] = stream.ReadByte();
                    data[3] = stream.ReadByte();
                    glm::vec3 tangent((float_t)data[0], (float_t)data[1], (float_t)data[2]);
                    tangent = (tangent * 2.0f) - 1.0f;
                    m_Tangents.push_back(ToLib(tangent));

                    data[0] = stream.ReadByte();
                    data[1] = stream.ReadByte();
                    data[2] = stream.ReadByte();
                    data[3] = stream.ReadByte();
                    glm::vec3 biTangent((float_t)data[0], (float_t)data[1], (float_t)data[2]);
                    biTangent = (biTangent * 2.0f) - 1.0f;
                    m_BiTangents.push_back(ToLib(biTangent));
                }
                else
                {
                    m_Tangents.emplace_back().ReadFromStream(stream);
                    m_BiTangents.emplace_back().ReadFromStream(stream);
                }
            }

            if ((m_Flags & EVBUFFlags::Color) != 0)
            {
                m_Colors.emplace_back().ReadFromStream(stream);
            }

            if ((m_Flags & EVBUFFlags::StaticLighting) != 0)
            {
                m_Colors.emplace_back().ReadFromStream(stream);
            }

            if ((m_Flags & EVBUFFlags::TexCoord) != 0)
            {
                if ((m_Flags & EVBUFFlags::TexCoordCompressed) != 0)
                {
                    uint16_t data[2];
                    data[0] = stream.ReadByte();
                    data[1] = stream.ReadByte();
                    glm::vec2 uv((float_t)data[0], (float_t)data[1]);
                    uv = uv / 2048.0f;
                    m_TexCoords.push_back(ToLib(uv));
                }
                else
                {
                    m_TexCoords.emplace_back().ReadFromStream(stream);
                }
            }
        }

        BaseChunk::EnsureEnd(stream);
    }

    std::string VBUF::ToString() const
    {
        std::string result = "Count = " + std::to_string(m_Count) + "\n";
        result += "Stride = " + std::to_string(m_Stride) + "\n";
        result += "Flags = " + std::string(VBUFFlagsToString(m_Flags)) + "\n";
        result += "Positions[" + std::to_string(m_Positions.size()) + "]\n";
        result += "Normals[" + std::to_string(m_Normals.size()) + "]\n";
        result += "Tangents[" + std::to_string(m_Tangents.size()) + "]\n";
        result += "BiTangents[" + std::to_string(m_BiTangents.size()) + "]\n";
        result += "Colors[" + std::to_string(m_Colors.size()) + "]\n";
        result += "Tex Coords[" + std::to_string(m_TexCoords.size()) + "]\n";
        result += "\n";

        //result += "Positions["+std::to_string(m_Positions.size())+"] = [";
        //for (uint32_t i = 0; i < m_Positions.size(); ++i)
        //{
        //    result += m_Positions[i].ToString() + std::string(", ");
        //    if ((i + 1) % 3 == 0)
        //    {
        //        result += "\n";
        //    }
        //}
        //result.resize(result.size() - 2);
        //result += "]";

        return result;
    }
}
