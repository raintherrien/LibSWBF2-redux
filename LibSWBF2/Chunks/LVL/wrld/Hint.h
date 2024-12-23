#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "Chunks/LVL/common/PROP.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct XFRM;
	using common::PROP;


	struct LIBSWBF2_API Hint_TYPE : public GenericChunk
	{
		uint16_t m_Type;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;		
		uint32_t GetHeader() override { return "TYPE"_m; }
	};


	struct LIBSWBF2_API Hint_INFO : public GenericChunk
	{
		std::shared_ptr<Hint_TYPE> p_Type;
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<XFRM> p_Transform;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		
		std::string ToString() const override;
		uint32_t GetHeader() override { return "INFO"_m; }
	};


	struct LIBSWBF2_API Hint : public GenericChunk
	{
		std::shared_ptr<Hint_INFO> p_Info;
		std::vector<std::shared_ptr<PROP>> m_Properties;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "Hint"_m; }
	};
}
