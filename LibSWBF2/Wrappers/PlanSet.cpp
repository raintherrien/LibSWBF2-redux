#include "pch.h"
#include "PlanSet.h"

#include "Chunks/LVL/plan/plan.h"

#include "InternalHelpers.h"


namespace LibSWBF2::Wrappers
{
	std::optional<PlanSet> PlanSet::FromChunk(std::shared_ptr<plan> planChunk)
	{
		PlanSet out;

		if (planChunk == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given planChunk was NULL!");
			return {};
		}

		out.p_Plan = planChunk;

		return out;
	}

	const std::vector<Hub>& PlanSet::GetHubs() const
	{
		return p_Plan->m_pNode->m_aHubs;
	}

	const std::vector<Connection>& PlanSet::GetConnections() const
	{
		return p_Plan->m_pArcs->m_aConnections;
	}
}
