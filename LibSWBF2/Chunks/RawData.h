#pragma once
#include "Chunks/GenericChunk.h"
#include <string>

namespace LibSWBF2::Chunks
{
	template<uint32_t Header>
	struct LIBSWBF2_API RawData : public GenericChunk
	{
		virtual ~RawData() override;

	protected:
		uint8_t* p_Data = nullptr;

	public:
		// returns false if there is no data
		bool GetData(const uint8_t*& data, size_t& size) const;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return Header; }
	};
}
