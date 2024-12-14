#pragma once
#include "Chunks/HeaderNames.h"
#include <fstream>
#include <atomic>


#include "FileReader.h"


namespace LibSWBF2
{
	class MemoryMappedReader : public FileReader
	{
	public:
		MemoryMappedReader();
		~MemoryMappedReader();

		bool Open(const std::string& File);
		ChunkHeader ReadChunkHeader(bool peek);
		ChunkSize ReadChunkSize();
		uint8_t ReadByte();
		bool ReadBytes(uint8_t* data, size_t length);
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
		bool SkipBytes(size_t Amount);
		void Close();

		bool CheckGood(size_t ReadSize);

	private:

		uint8_t* p_ReaderHead = nullptr;
		uint8_t* p_MappingBase = nullptr;
	};
}
