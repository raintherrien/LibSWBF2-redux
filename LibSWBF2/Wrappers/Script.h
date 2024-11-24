#pragma once
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::script
{
	struct scr_;
}


namespace LibSWBF2::Wrappers
{
	using Chunks::LVL::script::scr_;

	class Level;

	class LIBSWBF2_API Script
	{
	private:
		friend Level;

	private:
		scr_* p_Script;

	public:
		Script() = default;
		~Script() = default;

		static bool FromChunk(scr_* scriptChunk, Script& out);

		std::string GetName() const;
		bool GetData(const uint8_t*& data, size_t& size) const;
	};
}
