#pragma once
#include "Vector2.h"

#ifdef __cplusplus
namespace LibSWBF2::Types
{
#endif

	struct LIBSWBF2_API Vector3
	{
		float m_X;
		float m_Y;
		float m_Z;

#ifdef __cplusplus
		static const ChunkSize SIZE = sizeof(float) * 3;

		Vector3();
		Vector3(const float x, const float y, const float z);
		~Vector3();

		void WriteToStream(FileWriter& stream);
		void ReadFromStream(FileReader& stream);

		std::string ToString() const;
#endif
	};

#ifdef __cplusplus
} // namespace LibSWBF2::Types
#endif
