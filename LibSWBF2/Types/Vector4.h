#pragma once
#include "Vector3.h"

#ifdef __cplusplus
namespace LibSWBF2::Types
{
#endif

	struct LIBSWBF2_API Vector4
	{
		float m_X;
		float m_Y;
		float m_Z;
		float m_W;

#ifdef __cplusplus
		static const ChunkSize SIZE = sizeof(float) * 4;

		Vector4();
		Vector4(const float x, const float y, const float z, const float w);
		~Vector4();

		void WriteToStream(FileWriter& stream);
		void ReadFromStream(FileReader& stream);

		std::string ToString() const;
#endif
	};

#ifdef __cplusplus
} // namespace LibSWBF2::Vector4
#endif
