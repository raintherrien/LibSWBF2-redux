#pragma once
#include "Chunks/HeaderNames.h"
#include <atomic>
#include <string>

namespace LibSWBF2
{
	class FileReader
	{
	public:
		virtual ~FileReader() = default;

		virtual bool Open(const std::string& File) = 0;
		virtual ChunkHeader ReadChunkHeader(bool peek) = 0;
		virtual ChunkSize ReadChunkSize() = 0;
		virtual uint8_t ReadByte() = 0;
		virtual bool ReadBytes(void *dst, size_t count) = 0;
		virtual int32_t ReadInt32() = 0;
		virtual int16_t ReadInt16() = 0;
		virtual uint32_t ReadUInt32() = 0;
		virtual uint16_t ReadUInt16() = 0;
		virtual float_t ReadFloat() = 0;
		virtual std::string ReadString(size_t length) = 0;
		virtual std::string ReadString() = 0;// will read until the first null terminator
		virtual size_t GetPosition() = 0;
		virtual void SetPosition(size_t NewPosition) = 0;
		virtual size_t GetFileSize() = 0;
		virtual bool SkipBytes(size_t count) = 0;
		virtual void Close() = 0;

		virtual bool CheckGood(size_t ReadSize) = 0;

		size_t GetLatestChunkPosition() { return m_LatestChunkPos; }

		std::string GetFileName() { return m_FileName; }

	protected:

		size_t m_FileSize = 0;
		std::string m_FileName;
		
		std::atomic_size_t m_LatestChunkPos;
	};
}
