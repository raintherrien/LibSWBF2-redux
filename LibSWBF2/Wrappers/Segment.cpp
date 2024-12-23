#include "pch.h"
#include "Segment.h"
#include "Level.h"
#include "InternalHelpers.h"

#include "Chunks/LVL/modl/LVL.modl.h"
#include "Chunks/LVL/modl/modl.VBUF.h"
#include "Chunks/LVL/modl/IBUF.h"
#include "Chunks/LVL/modl/BMAP.h"
#include "Chunks/LVL/modl/modl.segm.h"


namespace LibSWBF2::Wrappers
{
	using LibSWBF2::Chunks::LVL::modl::SKIN;
	using LibSWBF2::Chunks::LVL::modl::BMAP;
	using LibSWBF2::Chunks::LVL::modl::modl;
	using LibSWBF2::Chunks::LVL::skel::skel;


	std::optional<Segment> Segment::FromChunk(std::shared_ptr<Level> mainContainer, std::shared_ptr<segm> segmentChunk)
	{
		Segment out;

		if (segmentChunk == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given SegmentChunk was NULL!");
			return {};
		}

		std::vector<std::shared_ptr<VBUF>>& vBuffs = segmentChunk->m_VertexBuffers;
		if (vBuffs.size() == 0)
		{
			LIBSWBF2_LOG_WARN("Segment Chunk does not contain any data!");
			return {};
		}

		out.p_Segment = segmentChunk;

		// find the VBUF we want to use, preferably one without compression
		out.p_VertexBuffer = nullptr;
		for (size_t i = 0; i < vBuffs.size(); ++i)
		{
			// TODO: better selection / sorting?
			if ((vBuffs[i]->m_Flags & EVBUFFlags::Position) != 0 &&
				(vBuffs[i]->m_Flags & EVBUFFlags::Normal) != 0 &&
				(vBuffs[i]->m_Flags & EVBUFFlags::TexCoord) != 0 &&
				(vBuffs[i]->m_Flags & EVBUFFlags::PositionCompressed) == 0 &&
				(vBuffs[i]->m_Flags & EVBUFFlags::NormalCompressed) == 0 &&
				(vBuffs[i]->m_Flags & EVBUFFlags::TexCoordCompressed) == 0)
			{
				out.p_VertexBuffer = vBuffs[i];
				break;
			}
		}

		if (out.p_VertexBuffer == nullptr)
		{
			out.p_VertexBuffer = vBuffs[0];
		}

		if (std::optional<Material> mat = Material::FromChunk(mainContainer, segmentChunk->p_Material)) {
			out.m_Material = *mat;
		} else {
			LIBSWBF2_LOG_WARN("Could not read Material!");
		}

		// Create weight buffer
		modl* parent = dynamic_cast<modl*>(out.p_Segment->GetParent());
		std::shared_ptr<skel> skeleton = mainContainer->FindSkeleton(parent->p_Name->m_Text);
		if (skeleton != nullptr)
		{
			// std::shared_ptr<SKIN> skin = out.p_Segment->p_Skin;
			std::shared_ptr<BMAP> boneMap = out.p_Segment->p_BoneMap;

			auto& boneIndicies = out.p_VertexBuffer -> m_BoneIndicies;
			auto& boneWeights  = out.p_VertexBuffer -> m_Weights;

			if (boneIndicies.size() > 0)
			{
				if (boneMap == nullptr)
				{
					LIBSWBF2_LOG_ERROR("Bone map missing...");
					return out;			
				}

				if (boneWeights.size() != 0)
				{
					for (int i = 0; i < boneIndicies.size(); i++)
					{
						uint8_t index =  boneIndicies[i].m_X;
						uint8_t index1 = boneIndicies[i].m_Y;
						uint8_t index2 = boneIndicies[i].m_Z;

						if (index  >= boneMap->m_IndexCount || 
							index1 >= boneMap->m_IndexCount ||
							index2 >= boneMap->m_IndexCount)
						{
							LIBSWBF2_LOG_ERROR("Softskin index ({},{},{}) is >= bone map length {}", index, index1, index2, boneMap->m_IndexCount);
						}
						else 
						{
							index = (uint8_t)  boneMap->m_IndexMap[index];
							index1 = (uint8_t) boneMap->m_IndexMap[index1];
							index2 = (uint8_t) boneMap->m_IndexMap[index2];
						}

						out.m_VertexWeights.push_back({ boneWeights[i].m_X, index });
						out.m_VertexWeights.push_back({ boneWeights[i].m_Y, index1 });
						out.m_VertexWeights.push_back({ boneWeights[i].m_Z, index2 });
					}
				}
				else 
				{
					for (int i = 0; i < boneIndicies.size(); i++)
					{
						uint8_t index = boneIndicies[i].m_X;

						if (index >= boneMap->m_IndexCount)
						{
							LIBSWBF2_LOG_ERROR("Index {} is >= bone map length {}", index, boneMap->m_IndexCount);
						}
						else 
						{
							index = (uint8_t) boneMap->m_IndexMap[index];
						}

						out.m_VertexWeights.push_back({ 1.0f, index });
					}
				}
			}
		}

		return out;
	}

	ETopology Segment::GetTopology() const
	{
		return p_Segment->p_Info->m_Topology;
	}

	const Material *Segment::GetMaterial() const
	{
		return &m_Material;
	}

	std::vector<uint16_t> Segment::GetIndexBuffer() const
	{
		return p_Segment->p_IndexBuffer->m_Indices;
	}

	std::vector<Vector3> Segment::GetVertexBuffer() const
	{
		return p_VertexBuffer->m_Positions;
	}

	std::vector<Vector3> Segment::GetNormalBuffer() const
	{
		return p_VertexBuffer->m_Normals;
	}

	std::vector<Vector2> Segment::GetUVBuffer() const
	{
		return p_VertexBuffer->m_TexCoords;
	}

	bool Segment::ContainsWeights() const
	{
		return p_Segment->p_Skin != nullptr;
	}

	bool Segment::GetVertexWeights(uint32_t& count, const VertexWeight*& weightBuffer) const
	{
		count = 0;
		weightBuffer = nullptr;

		if (!ContainsWeights())
			return false;

		count = (uint32_t)m_VertexWeights.size();
		weightBuffer = m_VertexWeights.data();
		return count > 0;
	}


	std::string Segment::GetBone() const
	{
		if (p_Segment -> p_Parent != nullptr)
		{
			return p_Segment -> p_Parent -> m_Text;
		}

		return "";
	}


	std::string Segment::GetTag() const
	{
		if (p_Segment -> p_Tag != nullptr)
		{
			return p_Segment -> p_Tag -> m_Text;			
		}

		return "";
	}


	bool Segment::IsPretransformed() const
	{
		auto flags = p_VertexBuffer -> m_Flags;
		return ((flags & EVBUFFlags::Unknown1) != 0 &&
				(flags & EVBUFFlags::BlendWeight) == 0);
	}
}
