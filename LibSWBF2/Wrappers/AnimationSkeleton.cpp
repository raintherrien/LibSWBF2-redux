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

	bool AnimationSkeleton::FromChunk(zaf_ *chunk, AnimationSkeleton &skelOut)
	{
		if (chunk == nullptr)
		{
			return false;
		}
		else 
		{
			if ((chunk -> p_Bin == nullptr) ||
				(chunk -> p_Bin -> p_Skeleton == nullptr) ||
				(chunk -> p_Name == nullptr))
			{
				return false;
			}
		}

		skelOut.p_AnimSkeleton = chunk;

		return true;
	}


	const std::string& AnimationSkeleton::GetName() const
	{
		static std::string Missing;
		if (p_AnimSkeleton -> p_Name != nullptr)
		{
			return p_AnimSkeleton -> p_Name -> m_Text;	
		}
		else 
		{
			return Missing;
		}
		
	}
	

	bool AnimationSkeleton::GetJoints(std::vector<Joint>& jointsOut) const
	{
		if ((p_AnimSkeleton -> p_Bin != nullptr) && (p_AnimSkeleton -> p_Bin -> p_Skeleton != nullptr))
		{
			jointsOut.clear();

			std::vector<TNOJ *>& jointChunks = p_AnimSkeleton -> p_Bin -> p_Skeleton -> m_Joints;
			for (int i = 0; i < jointChunks.size(); i++)
			{
				TNOJ *jC = jointChunks[i];
				jointsOut.push_back({jC -> m_BoneCRC, jC -> m_BaseRotation, jC -> m_BasePosition, jC -> m_ParentBoneCRC});
			}

			return true;
		}

		return false;
	}
}
