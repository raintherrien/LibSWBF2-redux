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

		plan* p_Plan;


	public:
		PlanSet() = default;
		~PlanSet() = default;

		static bool FromChunk(plan* planChunk, PlanSet& out);

		const std::vector<Hub>& GetHubs() const;
		const std::vector<Connection>& GetConnections() const;
	};
}
