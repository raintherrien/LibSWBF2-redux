#pragma once
#include "req.h"
#include "Types/Vector3.h"
#include "Types/Vector4.h"
#include <vector>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct zaf_;
}

namespace LibSWBF2::Wrappers
{
	using Types::Vector3;
	using Types::Vector4;

	#pragma pack(push,1)
	struct LIBSWBF2_API Joint
	{
		uint32_t m_BoneName;
		Types::Vector4 m_Rotation;
		Types::Vector3 m_Position;
		uint32_t m_ParentBoneName;
		// Might include the other 2 crcs if necessary
	};
	#pragma pack(pop)

	struct Level;

	class LIBSWBF2_API AnimationSkeleton
	{
		typedef LibSWBF2::Chunks::LVL::animation::zaf_ zaf_ ;
	private:
		friend Level;

		zaf_* p_AnimSkeleton;

	public:
		AnimationSkeleton() = default;
		~AnimationSkeleton() = default;

		static bool FromChunk(zaf_* animSkelChunk, AnimationSkeleton& out);

		const std::string& GetName() const;
		bool GetJoints(std::vector<Joint>& joints) const;
	};
}
