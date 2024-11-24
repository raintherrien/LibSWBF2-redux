#pragma once
#include "TranslationFrame.h"
#include "RotationFrame.h"
#include <vector>

namespace LibSWBF2
{
	class FileReader;
	class FileWriter;
}

namespace LibSWBF2::Types
{
	struct LIBSWBF2_API BoneFrames
	{
		uint32_t GetDataSize();
		void WriteToStream(FileWriter& stream);
		void ReadFromStream(FileReader& stream);

		CRCChecksum m_CRCchecksum = 0;		// The Bone this refers to
		uint32_t m_KeyFrameType = 0;

		std::vector<TranslationFrame> m_TranslationFrames;
		std::vector<RotationFrame> m_RotationFrames;
	};
}
