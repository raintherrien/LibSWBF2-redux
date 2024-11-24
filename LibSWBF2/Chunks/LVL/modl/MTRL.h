#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Color4u8.h"
#include "Types/Enums.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::modl
{
	struct LIBSWBF2_API MTRL : public GenericChunk<"MTRL"_m>
	{
	public:
		EMaterialFlags m_Flags;
		Color4u8 m_DiffuseColor;
		Color4u8 m_SpecularColor;
		uint32_t m_SpecularExponent;
		uint32_t m_Parameters[2];
		std::string m_AttachedLight;

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
	};
}
