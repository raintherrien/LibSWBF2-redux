#include "pch.h"
//#ifdef MEMORY_MAPPED_READER

#include "MemoryMappedReader.h"

#include "InternalHelpers.h"

#include <limits>
#include <memory>

#ifndef WIN32

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>

#endif


#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


namespace LibSWBF2
{
	using LibSWBF2::Logging::Logger;

	MemoryMappedReader::MemoryMappedReader()
	{
		m_LatestChunkPos = 0;
	    p_ReaderHead = nullptr;
	    p_MappingBase = nullptr;
	}

	MemoryMappedReader::~MemoryMappedReader()
	{
		try { Close(); } catch (...) {/*Do nothing if no mapping is set...*/}
	}

	bool MemoryMappedReader::Open(const std::string& File)
	{
		if (p_MappingBase != nullptr)
		{
			LIBSWBF2_LOG_ERROR("Already reading file: {}!", m_FileName);
			return false;
		}

		fs::path path = File;
		if (!fs::exists(path) || fs::is_directory(path))
		{
			LIBSWBF2_LOG_ERROR("File '{}' could not be found!", File);
			return false;
		}
		   
		m_FileSize = fs::file_size(path);
		m_FileName = path.string().c_str();

#ifndef WIN32
		int fd = open(m_FileName.c_str(), O_RDONLY);
		if (fd < 0)
		{
			LIBSWBF2_LOG_ERROR("Failed to open file: {}!", m_FileName);
			return false;
		}

		p_MappingBase = (uint8_t *) mmap(0,m_FileSize,PROT_READ,MAP_FILE|MAP_PRIVATE,fd,0);
		close(fd);

	    if (p_MappingBase == MAP_FAILED)
		{
			LIBSWBF2_LOG_ERROR("Memory map failed for file: {}!", m_FileName);
			return false;
	    }	  
#else
		HANDLE fileHandle = INVALID_HANDLE_VALUE, fileMappingHandle = NULL;

		try {
			fileHandle = CreateFileA(m_FileName.c_str(), GENERIC_READ, FILE_SHARE_READ,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (fileHandle == INVALID_HANDLE_VALUE)
			{
				LIBSWBF2_THROW("Failed to open file: {}!", m_FileName);
			}

			fileMappingHandle = CreateFileMapping(fileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
			if (fileMappingHandle == NULL)
			{
				LIBSWBF2_THROW("Failed to create file mapping object for file: {}!", m_FileName);
			}

			p_MappingBase = static_cast<uint8_t*>(MapViewOfFile(fileMappingHandle, FILE_MAP_READ, 0, 0, 0));
			if (p_MappingBase == nullptr)
			{
				LIBSWBF2_THROW("Memory map failed for file: {}!", m_FileName);
			}
		}
		catch (LibException e)
		{
			LIBSWBF2_LOG_ERROR("{}", e.what());
			if (fileMappingHandle != NULL) CloseHandle(fileMappingHandle);
			if (fileHandle != INVALID_HANDLE_VALUE) CloseHandle(fileHandle);
			throw;
		}

		CloseHandle(fileMappingHandle);
		CloseHandle(fileHandle);
#endif

		p_ReaderHead = p_MappingBase;

		LIBSWBF2_LOG_INFO("File '{}' ({} bytes) successfully opened.", m_FileName, m_FileSize);
		return true;
	}


	ChunkHeader MemoryMappedReader::ReadChunkHeader(bool peek)
	{
		ChunkHeader value;
		if (CheckGood(sizeof(ChunkHeader)))
		{
			auto pos = p_ReaderHead - p_MappingBase;

			memcpy(&value, p_ReaderHead, sizeof(value));
			p_ReaderHead += sizeof(value);

			// do not advance our reading position when peeking
			if (peek)
			{
				p_ReaderHead = p_MappingBase + pos;
			}

			m_LatestChunkPos = pos;
		}
		return value;
	}

	ChunkSize MemoryMappedReader::ReadChunkSize()
	{
		ChunkSize value = 0;
		if (CheckGood(sizeof(ChunkSize)))
		{
			value = *((uint32_t *) p_ReaderHead);
			p_ReaderHead += sizeof(value);
		}
		return value;
	}

	uint8_t MemoryMappedReader::ReadByte()
	{
		uint8_t value = 0;
		ReadBytes(&value, sizeof(value));
		return value;
	}

	bool MemoryMappedReader::ReadBytes(void *dst, size_t count)
	{
		if (CheckGood(count)) {
			memcpy(dst, p_ReaderHead, count);
			p_ReaderHead += count;

			return true;
		}
		return false;
	}

	int32_t MemoryMappedReader::ReadInt32()
	{
		int32_t value = 0;
		ReadBytes(&value, sizeof(value));
		return value;
	}

	int16_t MemoryMappedReader::ReadInt16()
	{
		int16_t value = 0;
		ReadBytes(&value, sizeof(value));
		return value;
	}

	uint32_t MemoryMappedReader::ReadUInt32()
	{
		uint32_t value = 0;
		ReadBytes(&value, sizeof(value));
		return value;
	}

	uint16_t MemoryMappedReader::ReadUInt16()
	{
		uint16_t value = 0;
		ReadBytes(&value, sizeof(value));
		return value;
	}

	float_t MemoryMappedReader::ReadFloat()
	{
		float_t value = 0.0f;
		ReadBytes(&value, sizeof(value));
		return value;
	}

	std::string MemoryMappedReader::ReadString(size_t length)
	{
		std::string value;
		if (CheckGood(length))
		{
			char* str = new char[length+1];
			memcpy((char *) str, (char *) p_ReaderHead, length);
			p_ReaderHead += length;

			str[length] = 0;
			value = str;
			delete[] str;
		}
		return value;
	}

	std::string MemoryMappedReader::ReadString()
	{
		char str[1024]; // should be enough
		uint8_t current = 1;
		for (uint16_t i = 0; CheckGood(1) && current != 0; ++i)
		{
			if (i >= 1024)
			{
				LIBSWBF2_LOG_WARN("Reading null terminated string exceeded buffer size!");
				break;
			}
			current = ReadByte();
			str[i] = current;
		}
		return str;
	}

	void MemoryMappedReader::Close()
	{
		if (p_MappingBase == nullptr)
		{
			LIBSWBF2_THROW("Nothing has been opened yet!");
		}

#ifndef WIN32
		munmap(p_MappingBase, m_FileSize);
#else
		UnmapViewOfFile(p_MappingBase);
#endif

		m_FileName = "";
		m_FileSize = 0;
		p_MappingBase = nullptr;
		p_ReaderHead = nullptr;
	}

	size_t MemoryMappedReader::GetPosition()
	{
		return (size_t) (p_ReaderHead - p_MappingBase);
	}

	void MemoryMappedReader::SetPosition(size_t NewPosition)
	{
		if (NewPosition > m_FileSize)
		{
			LIBSWBF2_LOG_WARN("Cannot set read position to {:#x} because it is out of range! Range: 0x00 - {:#x}", NewPosition, m_FileSize);
			return;
		}

		p_ReaderHead = p_MappingBase + NewPosition;
	}

	size_t MemoryMappedReader::GetFileSize()
	{
		return m_FileSize;
	}

	bool MemoryMappedReader::CheckGood(size_t ReadSize)
	{
		if (p_MappingBase == nullptr) 
		{
			LIBSWBF2_THROW("Cant read {:#x} bytes, memory mapping uninitialized!", ReadSize);
		}

		size_t current = (size_t) (p_ReaderHead - p_MappingBase);
		if (current + ReadSize > m_FileSize)
		{
			LIBSWBF2_THROW("Reading {:#x} bytes will end up out of file!  Current position: {:#x}  FileSize: {:#x}", ReadSize, current, m_FileSize);
		}

		return true;
	}

	bool MemoryMappedReader::SkipBytes(size_t count)
	{
		if (p_MappingBase == nullptr) {
			LIBSWBF2_THROW("Cant skip {:#x} bytes, memory mapping uninitialized!", count);
		}

		size_t offset = p_ReaderHead - p_MappingBase;
		if (offset > m_FileSize) {
			LIBSWBF2_THROW("Cant skip {:#x} bytes, we are already beyond the file!", count);
		}
		size_t desired = offset + count;
		if (desired > m_FileSize) {
			count -= desired - m_FileSize;
		}
		p_ReaderHead += count;

		return true;
	}
}

//#endif
