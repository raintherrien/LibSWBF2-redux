#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "PROP.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::common
{
	struct LIBSWBF2_API GenericClass : public GenericChunk
	{
	public:
		std::shared_ptr<STR<"BASE"_m>> p_Base;
		std::shared_ptr<STR<"TYPE"_m>> p_Type;

		std::vector<std::shared_ptr<PROP>> m_Properties;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};

	struct LIBSWBF2_API entc : public GenericClass
	{
		uint32_t GetHeader() override { return "entc"_m; }
	};

	struct LIBSWBF2_API ordc : public GenericClass
	{
		uint32_t GetHeader() override { return "ordc"_m; }
	};

	struct LIBSWBF2_API wpnc : public GenericClass
	{
		uint32_t GetHeader() override { return "wpnc"_m; }
	};

	struct LIBSWBF2_API expc : public GenericClass
	{
		uint32_t GetHeader() override { return "expc"_m; }
	};
} 
