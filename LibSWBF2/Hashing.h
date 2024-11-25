#pragma once
#include <unordered_map>
#include <string_view>

namespace LibSWBF2
{
	class CRC
	{
	public:
		LIBSWBF2_API static CRCChecksum CalcLowerCRC(const char* str);
	};

	class FNV
	{
	public:
		static FNVHash Hash(const std::string &str);
		static bool Lookup(FNVHash hash, std::string &result);

		static constexpr FNVHash HashConstexpr(const std::string_view str);

		static const std::unordered_map<FNVHash, std::string> &LookupTable();
	};


	constexpr FNVHash FNV::HashConstexpr(const std::string_view str)
	{
		constexpr uint32_t FNV_prime = 16777619;
		constexpr uint32_t offset_basis = 2166136261;

		uint32_t hash = offset_basis;

		for (auto c : str)
		{
			c |= 0x20;

			hash ^= c;
			hash *= FNV_prime;
		}

		return hash;
	}

	constexpr FNVHash operator""_fnv(const char* str, const std::size_t length)
	{
		return FNV::HashConstexpr({ str, length });
	}
}
