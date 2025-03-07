#pragma once
#include "Chunks/GenericChunk.h"
#include "Chunks/STR.h"
#include "config.NAME.h"
#include "DATA.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::config
{
	struct SCOP;
}

namespace LibSWBF2::Chunks::LVL::config
{
	template<uint32_t Header>
	struct LIBSWBF2_API ConfigChunk : public GenericChunk
	{
	public:
		std::shared_ptr<config_NAME> p_Hash;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;

		uint32_t GetHeader() override { return Header; }
	};

	struct LIBSWBF2_API lght : public ConfigChunk<"lght"_m> {};
	struct LIBSWBF2_API fx__ : public ConfigChunk<"fx__"_m> {};
	struct LIBSWBF2_API sky_ : public ConfigChunk<"sky_"_m> {};
 	struct LIBSWBF2_API bnd_ : public ConfigChunk<"bnd_"_m> {};
	struct LIBSWBF2_API prp_ : public ConfigChunk<"prp_"_m> {};
	struct LIBSWBF2_API path : public ConfigChunk<"path"_m> {};
	struct LIBSWBF2_API comb : public ConfigChunk<"comb"_m> {};
	struct LIBSWBF2_API snd_ : public ConfigChunk<"snd_"_m> {};
	struct LIBSWBF2_API mus_ : public ConfigChunk<"mus_"_m> {};
	struct LIBSWBF2_API ffx_ : public ConfigChunk<"ffx_"_m> {};
	struct LIBSWBF2_API tsr_ : public ConfigChunk<"tsr_"_m> {};
	struct LIBSWBF2_API hud_ : public ConfigChunk<"hud_"_m> {};
}
