#pragma once
#include "Types/Enums.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct zaa_;
}

namespace LibSWBF2::Wrappers
{

	class LIBSWBF2_API AnimationBank
	{
		typedef LibSWBF2::Chunks::LVL::animation::zaa_ zaa_ ;
	public:

		static bool FromChunk(zaa_ *chunk, AnimationBank &setOut);

		bool GetCurve(CRCChecksum anim, CRCChecksum bone, uint16_t component,
					std::vector<uint16_t> &frame_indices, std::vector<float_t> &frame_values) const;
	
		bool ContainsAnimation(CRCChecksum anim) const;

		std::vector<CRCChecksum> GetAnimations() const;
		std::vector<CRCChecksum> GetBones(CRCChecksum anim) const;
		
		bool GetAnimationMetadata(CRCChecksum anim, uint32_t &numFrames, uint32_t &numBones) const;

		std::string GetName() const;

		~AnimationBank();

		AnimationBank& operator=(const AnimationBank& other);


	private:

		zaa_ *p_AnimChunk = nullptr;
		class AnimDecompressor *p_Decompressor = nullptr;
	};
}
