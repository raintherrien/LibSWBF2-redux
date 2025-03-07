#include "pch.h"
#include "Script.h"
#include "InternalHelpers.h"

#include "Chunks/LVL/scr_/scr_.h"


namespace LibSWBF2::Wrappers
{
	std::optional<Script> Script::FromChunk(std::shared_ptr<scr_> scriptChunk)
	{
		Script out;

		if (scriptChunk == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given scriptChunk was NULL!");
			return {};
		}

		out.p_Script = scriptChunk;

		return out;
	}

	std::string Script::GetName() const
	{
		return p_Script->p_Name->m_Text;
	}

	bool Script::GetData(const uint8_t*& data, size_t& size) const
	{
		return p_Script->p_Body->GetData(data, size);
	}
}
