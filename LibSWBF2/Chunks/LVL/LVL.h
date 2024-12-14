#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Enums.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL
{
	namespace sound
	{
		struct Stream;
	}

	struct LIBSWBF2_API LVL : public GenericChunk<"ucfb"_m>
	{
	private:
		std::vector<FNVHash> m_SubLVLsToLoad;
		bool m_Lazy = false;

	public:
		friend struct lvl_;

		static LVL* Create();
		static void Destroy(LVL* lvl);

		void SetLazy(bool Lazy = true);

		void ReadFromStream(FileReader& stream) override;

		bool FindAndReadSoundStream(FileReader& stream, FNVHash soundStreamName, sound::Stream *& streamChunk);


		// All sub LVLs not specified in here won't be loaded!
		bool ReadFromFile(std::string Path, const std::vector<std::string>* subLVLsToLoad = nullptr);
	};
}
