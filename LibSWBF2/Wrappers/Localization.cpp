#include "pch.h"
#include "Localization.h"
#include "InternalHelpers.h"
#include "Hashing.h"

#include "Chunks/LVL/Locl/Locl.h"

#include <map>

namespace LibSWBF2::Wrappers
{
	class LocalizationMaps
	{
	public:
		std::unordered_map<FNVHash, size_t> m_HashToIndexMap;
	};

	Localization::Localization()
	{
		m_LocalizationMaps = new LocalizationMaps();
	}

	Localization::~Localization()
	{
		delete m_LocalizationMaps;
	}

	Localization::Localization(const Localization &other)
	{
		m_LocalizationMaps = new LocalizationMaps();
		*m_LocalizationMaps = *other.m_LocalizationMaps;
	}

	Localization& Localization::operator=(const Localization& other)
	{
		p_Locl = other.p_Locl;
		m_LocalizationMaps->m_HashToIndexMap = other.m_LocalizationMaps->m_HashToIndexMap;
		return *this;
	}

	Localization& Localization::operator=(Localization&& other)
	{
		p_Locl = other.p_Locl;
		m_LocalizationMaps = other.m_LocalizationMaps;
		other.m_LocalizationMaps = new LocalizationMaps();
		return *this;
	}

	std::optional<Localization> Localization::FromChunk(std::shared_ptr<Locl> loclChunk)
	{
		Localization out;

		if (loclChunk == nullptr)
		{
			LIBSWBF2_LOG_ERROR("Given loclChunk was NULL!");
			return {};
		}

		out.p_Locl = loclChunk;

		for (size_t i = 0; i < loclChunk->p_Body->m_LocalizeEntries.size(); ++i)
		{
			out.m_LocalizationMaps->m_HashToIndexMap.emplace(
				loclChunk->p_Body->m_LocalizeEntries[i].m_LocalizePathHash, 
				i
			);
		}

		return out;
	}

	const std::string& Localization::GetName() const
	{
		return p_Locl->p_Name->m_Text;
	}

	bool Localization::GetLocalizedWideString(const std::string& path, const wchar_t*& chars, uint32_t& count) const
	{
		if (path.empty())
		{
			LIBSWBF2_LOG_WARN("Given localize path was EMPTY!");
			return false;
		}

		FNVHash hashedPath = FNV::Hash(path);
		auto it = m_LocalizationMaps->m_HashToIndexMap.find(hashedPath);
		if (it != m_LocalizationMaps->m_HashToIndexMap.end())
		{
			std::wstring wideString = p_Locl->p_Body->m_LocalizeEntries[it->second].m_WideString;
			chars = wideString.c_str();
			count = (uint32_t)wideString.size();
			return true;
		}

		return false;
	}
}
