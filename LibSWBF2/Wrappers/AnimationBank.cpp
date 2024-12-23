#include "req.h"
#include "pch.h"

#include "AnimationBank.h"
#include "Chunks/LVL/zaa_/zaa_.h"

#include "InternalHelpers.h"



namespace LibSWBF2::Wrappers
{
	// AnimationBank

	using namespace Chunks::LVL::animation;

	std::optional<AnimationBank> AnimationBank::FromChunk(std::shared_ptr<zaa_> chunk)
	{
		AnimationBank setOut;

		if (chunk == nullptr)
		{
			return {};
		}
		else 
		{
			if ((chunk -> p_Bin == nullptr) ||
				(chunk -> p_Bin -> p_CompressedAnimData == nullptr) ||
				(chunk -> p_Bin -> p_JointAddresses == nullptr) ||
				(chunk -> p_Bin -> p_AnimsMetadata == nullptr) ||
				(chunk -> p_Name == nullptr))
			{
				return {};
			}
		}

		setOut.p_AnimChunk = chunk;
		setOut.p_Decompressor = std::make_unique<AnimDecompressor>(
								(void *) chunk -> p_Bin -> p_CompressedAnimData -> p_DataBuffer,
								chunk -> p_Bin -> p_CompressedAnimData -> m_DataBufferLength
							);

		return setOut;
	}

	std::string AnimationBank::GetName() const
	{
		return p_AnimChunk -> p_Name -> m_Text;
	}
	

	bool AnimationBank::ContainsAnimation(CRCChecksum animName) const
	{
		const auto &anims = p_AnimChunk->p_Bin->p_AnimsMetadata->m_AnimNameHashes;
		return std::find(std::begin(anims), std::end(anims), animName) != std::end(anims);
	}


	std::vector<CRCChecksum> AnimationBank::GetAnimations() const
	{
		return p_AnimChunk -> p_Bin -> p_AnimsMetadata -> m_AnimNameHashes;
	}


	bool AnimationBank::GetAnimationMetadata(CRCChecksum animCRC, uint32_t &numFrames, uint32_t &numBones) const
	{
		std::shared_ptr<MINA> metadata = p_AnimChunk -> p_Bin -> p_AnimsMetadata;	

		std::vector<CRCChecksum> &animCRCs = metadata -> m_AnimNameHashes;	

		for (int i = 0; i < animCRCs.size(); i++)
		{
			if (animCRCs[i] == animCRC)
			{
				numBones  = (uint32_t) metadata -> m_AnimBoneCounts[i];
				numFrames = (uint32_t) metadata -> m_AnimFrameCounts[i];	
				return true;
			}
		}

		return false;
	}


	std::vector<CRCChecksum> AnimationBank::GetBones(CRCChecksum animCRC) const
	{
		std::vector<CRCChecksum> boneHashes;

		std::shared_ptr<TNJA> index = p_AnimChunk -> p_Bin -> p_JointAddresses;
		std::shared_ptr<MINA> metadata = p_AnimChunk -> p_Bin -> p_AnimsMetadata;	


		std::vector<CRCChecksum> &animCRCs = metadata -> m_AnimNameHashes;	

		uint32_t TNJAOffset = 0;

		bool foundAnim = false;

		for (uint32_t i = 0; i < animCRCs.size(); i++)
		{
			if (animCRCs[i] == animCRC)
			{
				foundAnim = true;

				uint32_t num_bones = metadata -> m_AnimBoneCounts[i];

				for (uint32_t j = 0; j < num_bones; j++)
				{
					CRCChecksum currCRC = index -> m_BoneCRCs[TNJAOffset + j];
					if (std::find(std::begin(boneHashes), std::end(boneHashes), currCRC) == std::end(boneHashes))
					{
						boneHashes.push_back(currCRC);
					}
				}
			}
			else 
			{
				TNJAOffset += metadata -> m_AnimBoneCounts[i];
			}
		}

		if (!foundAnim)
		{
			LIBSWBF2_LOG_WARN("Unable to get bones of missing animation 0x{0:x} in bank '{1}'", animCRC, p_AnimChunk->p_Name->m_Text);
		}

		return boneHashes;
	}
	

	bool AnimationBank::GetCurve(CRCChecksum animName, CRCChecksum boneName, uint16_t component,
										std::vector<uint16_t> &frame_indices, std::vector<float_t> &frame_values) const
	{
		std::shared_ptr<TNJA> index = p_AnimChunk -> p_Bin -> p_JointAddresses;
		std::shared_ptr<TADA> data = p_AnimChunk -> p_Bin -> p_CompressedAnimData;
		std::shared_ptr<MINA> metadata = p_AnimChunk -> p_Bin -> p_AnimsMetadata;	

		bool decompStatus = false;

		std::vector<float_t> values;
		std::vector<uint16_t> indicies;

		std::vector<CRCChecksum> &animCRCs = metadata -> m_AnimNameHashes;	

		uint32_t TNJAOffset = 0;	

		for (uint32_t i = 0; i < animCRCs.size(); i++)
		{
			if (animCRCs[i] == animName)
			{
				uint32_t num_bones = metadata -> m_AnimBoneCounts[i];
				uint16_t num_frames = metadata -> m_AnimFrameCounts[i];	

				//Some zaabin files have duplicate entries for bones in TNJA.
				//The latter of the two is correct, so we try to find it first...

				uint32_t tempOffset = TNJAOffset;
				uint32_t foundBone = false;
				for (uint32_t j = 0; j < num_bones; j++)
				{
					if (index -> m_BoneCRCs[tempOffset] == boneName)
					{
						foundBone = true;
						TNJAOffset = tempOffset;
					}

					tempOffset++;
				}

				if (!foundBone)
				{
					return false;
				}


				uint32_t TADAOffset;

				if (component < 4)
				{
					p_Decompressor -> SetDecompressionParams();
					TADAOffset = index -> m_RotationOffsets[TNJAOffset * 4 + component];
				}
				else
				{
					float_t bias = index -> m_TranslationParams[4 * TNJAOffset + component - 4];
					float_t mult = index -> m_TranslationParams[4 * TNJAOffset + 3];

					p_Decompressor -> SetDecompressionParams(mult, bias);
					TADAOffset = index -> m_TranslationOffsets[TNJAOffset * 3 + component - 4];
				}

				decompStatus = p_Decompressor -> DecompressFromOffset(
										TADAOffset, num_frames, 
										frame_indices, 
										frame_values
									);	
			}
			else 
			{
				TNJAOffset += metadata -> m_AnimBoneCounts[i];
			}
		}

		return decompStatus;
	}
}

