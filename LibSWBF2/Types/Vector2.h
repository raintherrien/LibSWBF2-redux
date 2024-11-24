#pragma once
#include "../req.h"

#ifdef __cplusplus
namespace LibSWBF2
{
	class FileWriter;
	class FileReader;
}
#endif


#ifdef __cplusplus
namespace LibSWBF2::Types
{
#endif

	struct LIBSWBF2_API Vector2
	{
		float m_X;
		float m_Y;

#ifdef __cplusplus
		static const ChunkSize SIZE = sizeof(float) * 2;

		Vector2();
		Vector2(const float x, const float y);
		~Vector2();

		void WriteToStream(FileWriter& stream);
		void ReadFromStream(FileReader& stream);

		std::string ToString() const;
#endif
	};

#ifdef __cplusplus
} // namespace LibSWBF2::Types
#endif
