#pragma once
#include "Chunks/GenericChunk.h"
#include "Types/Vector3.h"
#include "Types/Matrix3x3.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks::LVL::skel
{
	using Types::Vector3;
	using Types::Matrix3x3;

	struct LIBSWBF2_API XFRM : public GenericChunk
	{
		std::vector<Matrix3x3> m_BoneRotations;
		std::vector<Vector3> m_BonePositions;

		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		std::string ToString() const override;
		uint32_t GetHeader() override { return "XFRM"_m; }
	};
}
