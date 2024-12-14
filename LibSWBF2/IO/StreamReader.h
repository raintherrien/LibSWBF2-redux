#pragma once
#include "FileReader.h"
#include "Chunks/HeaderNames.h"
#include <fstream>
#include <atomic>
#include <string>

namespace LibSWBF2
{
	class StreamReader : public FileReader
	{
	public:
		StreamReader();
		~StreamReader();

		bool Open(const std::string &File);
		ChunkHeader ReadChunkHeader(bool peek);
		ChunkSize ReadChunkSize();
		uint8_t ReadByte();
		bool ReadBytes(void *dst, size_t count);
		int32_t ReadInt32();
		int16_t ReadInt16();
		uint32_t ReadUInt32();
		uint16_t ReadUInt16();
		float_t ReadFloat();
		std::string ReadString(size_t length);
		std::string ReadString(); // will read until the first null terminator
		size_t GetPosition();
		void SetPosition(size_t NewPosition);
		size_t GetFileSize();
		bool SkipBytes(size_t count);
		void Close();

		bool CheckGood(size_t ReadSize);

	private:

		std::ifstream m_Reader;
	};
}
