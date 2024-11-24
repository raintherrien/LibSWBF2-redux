#include "pch.h"
#include "InternalHelpers.h"
#include "Logging/Logger.h"

namespace LibSWBF2
{
	Vector2 ToLib(const glm::vec2 vector)
	{
		return Vector2(vector.x, vector.y);
	}

	glm::vec3 ToGLM(const Vector3& vector)
	{
		return glm::vec3(vector.m_X, vector.m_Y, vector.m_Z);
	}

	Vector3 ToLib(const glm::vec3 vector)
	{
		return Vector3(vector.x, vector.y, vector.z);
	}

	glm::mat3 ToGLM(const Matrix3x3& matrix)
	{
		return glm::mat3(
			matrix.matrix[0][0], matrix.matrix[0][1], matrix.matrix[0][2],
			matrix.matrix[1][0], matrix.matrix[1][1], matrix.matrix[1][2],
			matrix.matrix[2][0], matrix.matrix[2][1], matrix.matrix[2][2]
		);
		//return glm::mat3(
		//	matrix.matrix[0][0], matrix.matrix[1][0], matrix.matrix[2][0],
		//	matrix.matrix[0][1], matrix.matrix[1][1], matrix.matrix[2][1],
		//	matrix.matrix[0][2], matrix.matrix[1][2], matrix.matrix[2][2]
		//);
	}

	Vector4 ToLib(const glm::quat& quaternion)
	{
		return Vector4(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	}

	template <typename V, typename T>
	std::vector<V> TriangleStripToTriangleList(std::vector<T>& indexBuffer, uint32_t offset)
	{
		std::vector<V> result;
		V a,b,c;

		for (int i = 0; i < indexBuffer.size() - 2; i++)
		{
			a = indexBuffer[i]   + (V) offset;
			b = indexBuffer[i+1] + (V) offset;
			c = indexBuffer[i+2] + (V) offset; 

			if (a != b && b != c && a != c)	//Catch degenerate 
			{
				if (i % 2 != 0) //swap clockwiseness 
				{
					std::swap(a,b);
				}

				result.push_back(a);
				result.push_back(b);
				result.push_back(c);
			}
		}

		return result;
	}

	template std::vector<uint32_t> TriangleStripToTriangleList<uint32_t,uint32_t>(std::vector<uint32_t>& indexBuffer, uint32_t offset);
	template std::vector<uint32_t> TriangleStripToTriangleList<uint32_t,uint16_t>(std::vector<uint16_t>& indexBuffer, uint32_t offset);
	template std::vector<uint16_t> TriangleStripToTriangleList<uint16_t,uint16_t>(std::vector<uint16_t>& indexBuffer, uint32_t offset);


	template <typename V, typename T>
	std::vector<V> TriangleFanToTriangleList(std::vector<T>& indexBuffer, uint32_t offset)
	{
		std::vector<V> result;
		V a,b,c;

		if (indexBuffer.size() < 3)
		{
			return result;
		}

		for (int i = 2; i < indexBuffer.size(); i++)
		{
			a = indexBuffer[i-1] + (V) offset;
			b = indexBuffer[i]   + (V) offset;
			c = indexBuffer[0]   + (V) offset; 

			if (a != b && b != c && a != c)	//Catch degenerate 
			{
				result.push_back(a);
				result.push_back(b);
				result.push_back(c);
			}
		}

		return result;
	}

	template std::vector<uint32_t> TriangleFanToTriangleList<uint32_t,uint32_t>(std::vector<uint32_t>& indexBuffer, uint32_t offset);
	template std::vector<uint32_t> TriangleFanToTriangleList<uint32_t,uint16_t>(std::vector<uint16_t>& indexBuffer, uint32_t offset);
	template std::vector<uint16_t> TriangleFanToTriangleList<uint16_t,uint16_t>(std::vector<uint16_t>& indexBuffer, uint32_t offset);



	Vector4 MatrixToQuaternion(const Matrix3x3& matrix)
	{
		glm::mat3 mat = ToGLM(matrix);
		glm::quat quat = glm::quat(mat);
		return ToLib(quat);
	}

	std::string ToLower(const std::string &name)
	{
		std::string result = name;
		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return result;
	}
}
