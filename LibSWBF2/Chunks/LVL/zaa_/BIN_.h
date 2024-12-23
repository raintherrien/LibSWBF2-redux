#pragma once
#include "Chunks/GenericChunk.h"
#include "TADA.h"
#include "MINA.h"
#include "TNJA.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::animation
{
	struct LIBSWBF2_API BIN_ : public GenericChunk
	{

		//Always 8 for SWBF2 lvls, will abort read if a
		//diferent one is encountered.  Confirmed by 
		//ZAT files
		uint32_t m_Version; 

		uint32_t m_NumBones; 

		//Length of data buffer in TADA
		uint32_t m_DataBufferLength;

		uint16_t m_NumAnimations;

		//ZenAsset can pack a bunch of extra debug data
		//into TADA, essentially writing decompressed anims.
		//Will abort if not 0.
		uint16_t m_DebugLevel;

		std::shared_ptr<MINA> p_AnimsMetadata;
		std::shared_ptr<TNJA> p_JointAddresses;
		std::shared_ptr<TADA> p_CompressedAnimData;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "BIN_"_m; }
	};
}
