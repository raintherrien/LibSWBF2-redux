#pragma once
#include <string>

namespace LibSWBF2
{
	class FileReader;
	class FileWriter;
}

namespace LibSWBF2::Types
{
	struct LIBSWBF2_API Animation
	{
		static const int ANIMATION_NAME_STR_SIZE = 64;
		static const ChunkSize SIZE = ANIMATION_NAME_STR_SIZE + sizeof(float_t) + (sizeof(uint32_t) * 3);

		void WriteToStream(FileWriter& stream);
		void ReadFromStream(FileReader& stream);

		std::string m_AnimationName;
		float_t m_FrameRate = 0.0f;
		uint32_t m_PlayStyle = 0;
		uint32_t m_FirstFrame = 0;
		uint32_t m_LastFrame = 0;
	};
}
