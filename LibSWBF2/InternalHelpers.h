// THis file should NEVER be included in exposed headers!
#pragma once
#include "Chunks/HeaderNames.h"
#include "Chunks/BaseChunk.h"
#include "Types/Vector4.h"
#include "Types/Matrix3x3.h"
#include "Logging/Logger.h"
#include "Exceptions.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat3x3.hpp>
#include <fmt/core.h>
#include <fmt/format.h>

#include <unordered_map>
#include <vector>

namespace LibSWBF2
{
	using Types::Vector2;
	using Types::Vector3;
	using Types::Vector4;
	using Types::Matrix3x3;

	Vector2 ToLib(const glm::vec2 vector);
	glm::vec3 ToGLM(const Vector3& vector);
	Vector3 ToLib(const glm::vec3 vector);
	glm::mat3 ToGLM(const Matrix3x3& matrix);
	Vector4 ToLib(const glm::quat& quaternion);

	// Convert an index buffer from Triangle Strip format to Triangle List format
	// optional: offset is added to each individual index.
	template <typename V, typename T>
	std::vector<V> TriangleStripToTriangleList(std::vector<T> &indexBuffer, uint32_t offset=0);

	// Convert an index buffer from Triangle Fan format to Triangle List format
	// optional: offset is added to each individual index.
	// (so far only used for collision mesh extraction)
	template <typename V, typename T>
	std::vector<V> TriangleFanToTriangleList(std::vector<T> &indexBuffer, uint32_t offset=0);


	Vector4 MatrixToQuaternion(const Matrix3x3& matrix);

	std::string ToLower(const std::string &name);


	namespace Chunks::LVL::skel
	{
		struct skel;
	}

	namespace Wrappers
	{
		class MapsWrapper
		{
		public:
			std::unordered_map<FNVHash, size_t> TextureNameToIndex;
			std::unordered_map<FNVHash, size_t> ModelNameToIndex;
			std::unordered_map<FNVHash, size_t> WorldNameToIndex;
			std::unordered_map<FNVHash, size_t> TerrainNameToIndex;
			std::unordered_map<FNVHash, size_t> ScriptNameToIndex;
			std::unordered_map<FNVHash, size_t> LocalizationNameToIndex;
			std::unordered_map<FNVHash, size_t> EntityClassTypeToIndex;
			std::unordered_map<FNVHash, Chunks::LVL::skel::skel*> SkeletonNameToSkel;
			std::unordered_map<FNVHash, size_t> AnimationBankNameToIndex;
			std::unordered_map<FNVHash, size_t> AnimationSkeletonNameToIndex;
			std::unordered_map<FNVHash, size_t> SoundHashToIndex;
			std::unordered_map<FNVHash, size_t> ConfigHashToIndex;
			std::unordered_map<FNVHash, size_t> SoundStreamHashToIndex;
			std::unordered_map<FNVHash, size_t> SoundBankHashToIndex;
		};

		class SoundMapsWrapper
		{
		public:
			std::unordered_map<FNVHash, size_t> SoundHashToIndex;
		};
	}
}

#ifdef WIN32
#define LIBSWBF2_DIRDELIM '\\'
#else
#define LIBSWBF2_DIRDELIM '/'
#endif
#define LIBSWBF2_FILENAME (strrchr(__FILE__, LIBSWBF2_DIRDELIM) ? strrchr(__FILE__, LIBSWBF2_DIRDELIM) + 1 : __FILE__)
#define LIBSWBF2_LOG_DEBUG(...) LibSWBF2::Logging::Logger::Log(fmt::format(__VA_ARGS__), LibSWBF2::ELogType::Debug, __LINE__, LIBSWBF2_FILENAME, __func__)
#define LIBSWBF2_LOG_INFO(...) LibSWBF2::Logging::Logger::Log(fmt::format(__VA_ARGS__), LibSWBF2::ELogType::Info, __LINE__, LIBSWBF2_FILENAME, __func__)
#define LIBSWBF2_LOG_WARN(...) LibSWBF2::Logging::Logger::Log(fmt::format(__VA_ARGS__), LibSWBF2::ELogType::Warning, __LINE__, LIBSWBF2_FILENAME, __func__)
#define LIBSWBF2_LOG_ERROR(...) LibSWBF2::Logging::Logger::Log(fmt::format(__VA_ARGS__), LibSWBF2::ELogType::Error, __LINE__, LIBSWBF2_FILENAME, __func__)

#define LIBSWBF2_THROW(...) throw LibSWBF2Exception(fmt::format("{}:{}:{} {}", LIBSWBF2_FILENAME, __LINE__, __func__, fmt::format(__VA_ARGS__)))
