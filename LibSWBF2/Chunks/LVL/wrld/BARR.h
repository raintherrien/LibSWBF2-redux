#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::wrld
{
	struct XFRM;
	struct SIZE;

	struct LIBSWBF2_API BARR_FLAG : public GenericChunk
	{
		uint32_t m_Flag;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;		

		uint32_t GetHeader() override { return "FLAG"_m; }
	};


	struct LIBSWBF2_API BARR_INFO : public GenericChunk
	{
		std::shared_ptr<STR<"NAME"_m>> p_Name;
		std::shared_ptr<XFRM> p_Transform;
		std::shared_ptr<SIZE> p_Size;

		std::shared_ptr<BARR_FLAG> p_Flag;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
		
		std::string ToString() const override;

		uint32_t GetHeader() override { return "INFO"_m; }
	};


	struct LIBSWBF2_API BARR : public GenericChunk
	{
		std::shared_ptr<BARR_INFO> p_Info;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return "BARR"_m; }
	};
}
