#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Matrix3x3.h"
#include <string>

namespace LibSWBF2::Chunks::LVL::wrld
{
	using Types::Matrix3x3;

	struct LIBSWBF2_API XFRM : public GenericChunk
	{
		Matrix3x3 m_RotationMatrix;
		Vector3 m_Position;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "XFRM"_m; }
	};
}
