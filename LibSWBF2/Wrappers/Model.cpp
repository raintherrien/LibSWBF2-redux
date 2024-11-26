#include "pch.h"
#include "Level.h"
#include "Model.h"
#include "InternalHelpers.h"

#include "Chunks/LVL/modl/LVL.modl.h"
#include "Chunks/LVL/skel/skel.h"

static const char *empty = "";

namespace LibSWBF2::Wrappers
{
	bool Model::FromChunk(Level* mainContainer, modl* modelChunk, Model& out)
	{
		if (mainContainer == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given mainContainer was NULL!");
			return false;
		}
		if (modelChunk == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given modelChunk was NULL!");
			return false;
		}

		out.p_Model = modelChunk;
		out.p_Skeleton = mainContainer->FindSkeleton(out.p_Model->p_Name->m_Text);

		std::vector<segm*>& segments = modelChunk->m_Segments;
		for (size_t i = 0; i < segments.size(); ++i)
		{
			Segment segment;
			if (Segment::FromChunk(mainContainer, segments[i], segment))
			{
				out.m_Segments.push_back(segment);
			}
		}

		// Create m_Bones list
		if (out.p_Skeleton != nullptr) {
			uint32_t count = out.p_Skeleton->p_Info->m_BoneCount;
			std::vector<std::string>& names = out.p_Skeleton->p_BoneNames->m_Texts;
			std::vector<std::string>& parents = out.p_Skeleton->p_BoneParents->m_Texts;
			std::vector<Matrix3x3>& rotations = out.p_Skeleton->p_BoneTransforms->m_BoneRotations;
			std::vector<Vector3>& positions = out.p_Skeleton->p_BoneTransforms->m_BonePositions;


			int offset = out.IsSkeletonBroken() ? 1 : 0;

			//for (size_t i = 0; i < p_Skeleton->p_Info->m_BoneCount; ++i)
			for (size_t i = 0; i < out.p_Skeleton->p_BoneNames->m_Texts.size(); ++i)
			{
				// first entry (root) doesn't have a parent
				out.m_Bones.push_back({ names[i], i > 0 ? parents[i - 1] : empty, positions[i + offset], MatrixToQuaternion(rotations[i + offset]) });
			}
		}

		return true;
	}

	std::string Model::GetName() const
	{
		return p_Model->p_Name->m_Text;
	}

	std::vector<Segment> Model::GetSegments() const
	{
		return m_Segments;
	}

	bool Model::IsSkeletalMesh() const
	{
		if (p_Skeleton == nullptr)
			return false;

		for (size_t i = 0; i < m_Segments.size(); ++i)
		{
			if (m_Segments[i].ContainsWeights())
			{
				return true;
			}
		}
		return false;
	}


	bool Model::IsSkeletonBroken() const
	{
		if (p_Skeleton == nullptr)
			return true;

		uint32_t count = p_Skeleton->p_Info->m_BoneCount;
		std::vector<std::string>& names = p_Skeleton->p_BoneNames->m_Texts;
		std::vector<std::string>& parents = p_Skeleton->p_BoneParents->m_Texts;
		std::vector<Matrix3x3>& rotations = p_Skeleton->p_BoneTransforms->m_BoneRotations;
		std::vector<Vector3>& positions = p_Skeleton->p_BoneTransforms->m_BonePositions;

		// there's always one less parent, because root (DummyRoot) doesn't have one =P
		size_t equalParents = parents.size() + 1;
		if (count != names.size() || count != equalParents || count != rotations.size() || count != positions.size())
		{
			LIBSWBF2_LOG_ERROR(
				"Broken Skeleton! Sizes don't match up!\n"
				"\tCount: {}\n"
				"\tNames: {}\n"
				"\tParents (-1): {}\n"
				"\tRotations: {}\n"
				"\tPositions: {}",
				count,
				names.size(),
				parents.size(),
				rotations.size(),
				positions.size()
			);

			return true;
		}

		return false;
	}

	std::vector<Bone> Model::GetBones() const
	{
		return m_Bones;
	}

	const CollisionMesh *Model::GetCollisionMesh() const
	{
		return &m_CollisionMesh;
	}


	std::vector<CollisionPrimitive> Model::GetCollisionPrimitives() const
	{
		return m_CollisionPrimitives;
	}
}
