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

	// AnimDecompressor

	class AnimDecompressor
	{

	public:

		AnimDecompressor(void * buf, size_t len) : p_Buffer((int8_t *) buf), m_Length(len){}
		AnimDecompressor() : p_Buffer(nullptr), m_Length(0) {}

		void SetDecompressionParams(float_t mult = 1.0f / 2047.0f, float_t offset = 0.0) const
		{
			m_Bias = offset;
			m_Multiplier = mult;
		}

		bool DecompressFromOffset(size_t offset, uint16_t num_frames, 
								std::vector<uint16_t> &frame_indicies, 
								std::vector<float_t> &frame_values) const
		{
			std::vector<uint16_t> indicies;
			std::vector<float_t> values;

			m_ReadHead = offset;

			uint16_t frame_counter = 0;

			int16_t shortVal;
			int8_t byteVal;
			uint8_t holdDuration;

			float accum = 0.0f;
			
			while (frame_counter < num_frames)
			{
				if (!ReadInt16(shortVal)) return false;

				accum = m_Bias + m_Multiplier * (float) shortVal;

				indicies.push_back(frame_counter);
				values.push_back(accum);

				frame_counter++;


				while (frame_counter < num_frames)
				{
					if (!ReadInt8(byteVal)) return false;

					// Signals to hold accumulator for x frames,
					// x specified by the next (unsigned) byte.
					if (byteVal == -0x80)
					{
						if (!ReadUInt8(holdDuration)) return false;

					#ifndef _ANIM_DEBUG
						frame_counter += holdDuration;
					#else
						for (int i = 0; i < holdDuration; i++)
						{
							indicies.push_back(frame_counter);
							values.push_back(accum);

							frame_counter++;
						}
					#endif
					}

					// Signals to reset the accumulator to the value
					// of the next decompressed short.
					else if (byteVal == -0x7f)
					{
						break;
					}

					// Increment the accumulator by the value
					// of the next decompressed byte.  Decomp here
					// does not apply the offset, only the multiplier.
					else 
					{
						accum += m_Multiplier * (float) byteVal;

						indicies.push_back(frame_counter);
						values.push_back(accum);

						frame_counter++;
					}
				}
			}

			frame_indicies = std::move(indicies);
			frame_values   = std::move(values); 

			return true;
		}


	private:

		int8_t *p_Buffer;
		size_t m_Length;

		mutable size_t m_ReadHead;
		mutable float_t m_Bias, m_Multiplier;

		inline bool ReadInt16(int16_t &val) const
		{
			if (m_ReadHead < m_Length - 1)
			{
				val = *((int16_t *) (m_ReadHead + p_Buffer));
				m_ReadHead += 2;
				return true;
			}
			return false;
		}

		inline bool ReadInt8(int8_t &val) const
		{
			if (m_ReadHead < m_Length)
			{
				val = *(m_ReadHead + p_Buffer);
				m_ReadHead++;
				return true;
			}
			return false;
		}

		inline bool ReadUInt8(uint8_t &val) const
		{
			if (m_ReadHead < m_Length)
			{
				val = *((uint8_t *) (m_ReadHead + p_Buffer));
				m_ReadHead++;
				return true;
			}
			return false;
		}
	};

	class LIBSWBF2_API AnimationBank
	{
		typedef LibSWBF2::Chunks::LVL::animation::zaa_ zaa_ ;
	public:

		static std::optional<AnimationBank> FromChunk(std::shared_ptr<zaa_> chunk);

		bool GetCurve(CRCChecksum anim, CRCChecksum bone, uint16_t component,
					std::vector<uint16_t> &frame_indices, std::vector<float_t> &frame_values) const;
	
		bool ContainsAnimation(CRCChecksum anim) const;

		std::vector<CRCChecksum> GetAnimations() const;
		std::vector<CRCChecksum> GetBones(CRCChecksum anim) const;
		
		bool GetAnimationMetadata(CRCChecksum anim, uint32_t &numFrames, uint32_t &numBones) const;

		std::string GetName() const;

	private:

		std::shared_ptr<zaa_> p_AnimChunk;
		std::unique_ptr<AnimDecompressor> p_Decompressor;
	};
}
