#include "req.h"
#include "pch.h"

#include "AnimationSkeleton.h"
#include "Level.h"
#include "Chunks/LVL/zaf_/zaf_.h"
#include "Chunks/LVL/zaf_/TNOJ.h"
#include "Chunks/LVL/zaf_/BIN_.h"
#include "Chunks/LVL/zaf_/LEKS.h"

#include "InternalHelpers.h"



namespace LibSWBF2::Wrappers
{
	using namespace Chunks::LVL::animation;

	std::optional<AnimationSkeleton> AnimationSkeleton::FromChunk(std::shared_ptr<zaf_> chunk)
	{
		AnimationSkeleton skelOut;

		if (chunk == nullptr)
		{
			return {};
		}
		else 
		{
			if ((chunk -> p_Bin == nullptr) ||
				(chunk -> p_Bin -> p_Skeleton == nullptr) ||
				(chunk -> p_Name == nullptr))
			{
				return {};
			}
		}

		skelOut.p_AnimSkeleton = chunk;

		return skelOut;
	}


	std::string AnimationSkeleton::GetName() const
	{
		if (p_AnimSkeleton -> p_Name != nullptr)
		{
			return p_AnimSkeleton -> p_Name -> m_Text;	
		}
		else 
		{
			return "";
		}
		
	}
	

	bool AnimationSkeleton::GetJoints(std::vector<Joint>& jointsOut) const
	{
		if ((p_AnimSkeleton -> p_Bin != nullptr) && (p_AnimSkeleton -> p_Bin -> p_Skeleton != nullptr))
		{
			jointsOut.clear();

			std::vector<std::shared_ptr<TNOJ>>& jointChunks = p_AnimSkeleton -> p_Bin -> p_Skeleton -> m_Joints;
			for (int i = 0; i < jointChunks.size(); i++)
			{
				std::shared_ptr<TNOJ> jC = jointChunks[i];
				jointsOut.push_back({jC -> m_BoneCRC, jC -> m_BaseRotation, jC -> m_BasePosition, jC -> m_ParentBoneCRC});
			}

			return true;
		}

		return false;
	}
}
