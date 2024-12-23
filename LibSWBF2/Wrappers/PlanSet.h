#pragma once
#include "req.h"
#include "Types/Planning.h"
#include <vector>

namespace LibSWBF2::Chunks::plan
{
	struct plan;
}


namespace LibSWBF2::Wrappers
{
	using Types::Hub;
	using Types::Connection;

	class Level;

	class LIBSWBF2_API PlanSet
	{
	typedef LibSWBF2::Chunks::plan::plan plan;

	private:
		friend Level;

		std::shared_ptr<plan> p_Plan;

	public:
		static std::optional<PlanSet> FromChunk(std::shared_ptr<plan> planChunk);

		const std::vector<Hub>& GetHubs() const;
		const std::vector<Connection>& GetConnections() const;
	};
}
