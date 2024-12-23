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

	struct LIBSWBF2_API LVL : public GenericChunk
	{
	private:
		std::vector<FNVHash> m_SubLVLsToLoad;

	public:
		friend struct lvl_;

		void ReadFromStream(FileReader& stream) override;

		std::shared_ptr<sound::Stream> FindAndReadSoundStream(FileReader& stream, FNVHash soundStreamName);


		// All sub LVLs not specified in here won't be loaded!
		bool ReadFile(std::string Path, const std::vector<std::string>* subLVLsToLoad = nullptr);

		uint32_t GetHeader() override { return "ucfb"_m; }
	};
}
