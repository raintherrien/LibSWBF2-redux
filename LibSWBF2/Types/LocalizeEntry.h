#pragma once
#include <string>

namespace LibSWBF2
{
	class FileReader;
	class FileWriter;
}

namespace LibSWBF2::Types
{
	struct LIBSWBF2_API LocalizeEntry
	{
		FNVHash m_LocalizePathHash;		// hashed path, e.g. "common.quit2windows"
		uint16_t m_EntrySize;			// complete size of this entry
		std::wstring m_WideString;

		void RefreshSize();
		void WriteToStream(FileWriter& stream);
		void ReadFromStream(FileReader& stream);
	};
}
