#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Planning.h"
#include <string>
#include <vector>

using LibSWBF2::Types::Hub;
using LibSWBF2::Types::Connection;


namespace LibSWBF2::Chunks::plan
{

	struct LIBSWBF2_API PLAN_INFO : public GenericChunk
	{
	public:
		uint16_t m_iHubCount;
		uint16_t m_iConnectionCount;

		void ReadFromStream(FileReader& stream) override;
		std::string ToString() const override;

		uint32_t GetHeader() override { return "INFO"_m; }
	};

	struct LIBSWBF2_API NODE : public GenericChunk
	{
	public:

		NODE(uint16_t count);

		uint16_t m_iCount;
		std::vector<Hub> m_aHubs;

		void ReadFromStream(FileReader& stream) override;
		std::string ToString() const override;

		uint32_t GetHeader() override { return "NODE"_m; }
	};

	struct LIBSWBF2_API ARCS : public GenericChunk
	{
	public:
		ARCS(uint16_t count);

		uint16_t m_iCount;
		std::vector<Connection> m_aConnections;

		void ReadFromStream(FileReader& stream) override;
		std::string ToString() const override;

		uint32_t GetHeader() override { return "ARCS"_m; }
	};

	struct LIBSWBF2_API plan : public GenericChunk
	{
		std::shared_ptr<PLAN_INFO> m_pInfo;
		std::shared_ptr<NODE> m_pNode;
		std::shared_ptr<ARCS> m_pArcs;

		void ReadFromStream(FileReader& stream) override;

		uint32_t GetHeader() override { return "plan"_m; }
	};

	struct LIBSWBF2_API PLNS : public GenericChunk
	{
		uint32_t GetHeader() override { return "PLNS"_m; }
	};
}
