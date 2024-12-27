#include "pch.h"
//#ifndef MEMORY_MAPPED_READER

#include "StreamReader.h"

#include "InternalHelpers.h"


namespace LibSWBF2
{
	using LibSWBF2::Logging::Logger;

	StreamReader::StreamReader()
	{
		LIBSWBF2_LOG_INFO("StreamReader()");
		m_LatestChunkPos = 0;
	}

	StreamReader::~StreamReader()
	{
		LIBSWBF2_LOG_INFO("~StreamReader()");
		try { Close(); } catch (...) {}
	}

	bool StreamReader::Open(const std::string &File)
	{
		LIBSWBF2_LOG_INFO("StreamReader::Open('{}')", File);
		m_Reader.open(File, std::ofstream::in | std::ofstream::binary | std::ofstream::ate);
		bool success = m_Reader.good() && m_Reader.is_open();

		if (!success)
		{
			LIBSWBF2_LOG_WARN("File '{}' could not be opened", File);
			m_Reader.close();
			return false;
		}

		m_FileName = File;
		m_FileSize = (size_t)m_Reader.tellg();
		m_Reader.seekg(0);

		LIBSWBF2_LOG_INFO("File '{}' ({} bytes) successfully opened", m_FileName, m_FileSize);
		return true;
	}

	ChunkHeader StreamReader::ReadChunkHeader(bool peek)
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadChunkHeader({})", peek);
		ChunkHeader value;
		if (CheckGood(sizeof(ChunkHeader)))
		{
			auto pos = m_Reader.tellg();
			m_Reader.read((char*)&value, sizeof(value));

			// do not advance our reading position when peeking
			if (peek)
			{
				m_Reader.seekg(pos);
			}

			m_LatestChunkPos = pos;
		}
		return value;
	}

	ChunkSize StreamReader::ReadChunkSize()
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadChunkSize()");
		ChunkSize value = 0;
		if (CheckGood(sizeof(ChunkSize)))
		{
			m_Reader.read((char*)&value, sizeof(value));
		}
		return value;
	}

	uint8_t StreamReader::ReadByte()
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadByte()");
		uint8_t value = 0;
		if (CheckGood(sizeof(uint8_t)))
		{
			m_Reader.read((char*)&value, sizeof(value));
		}
		return value;
	}

	bool StreamReader::ReadBytes(void *data, size_t length)
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadBytes()");
		if (CheckGood(length))
		{
			m_Reader.read((char*)data, length);
			return true;
		}
		return false;
	}

	int32_t StreamReader::ReadInt32()
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadInt32()");
		int32_t value = 0;
		if (CheckGood(sizeof(int32_t)))
		{
			m_Reader.read((char*)&value, sizeof(value));
		}
		return value;
	}

	int16_t StreamReader::ReadInt16()
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadInt16()");
		int16_t value = 0;
		if (CheckGood(sizeof(int16_t)))
		{
			m_Reader.read((char*)&value, sizeof(value));
		}
		return value;
	}

	uint32_t StreamReader::ReadUInt32()
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadUInt32()");
		uint32_t value = 0;
		if (CheckGood(sizeof(uint32_t)))
		{
			m_Reader.read((char*)&value, sizeof(value));
		}
		return value;
	}

	uint16_t StreamReader::ReadUInt16()
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadUInt16()");
		uint16_t value = 0;
		if (CheckGood(sizeof(uint16_t)))
		{
			m_Reader.read((char*)&value, sizeof(value));
		}
		return value;
	}

	float_t StreamReader::ReadFloat()
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadFloat()");
		float_t value = 0.0f;
		if (CheckGood(sizeof(float_t)))
		{
			m_Reader.read((char*)&value, sizeof(value));
		}
		return value;
	}

	std::string StreamReader::ReadString(size_t length)
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadString('{}')", length);
		std::string value;
		if (CheckGood(length))
		{
			char* str = new char[length+1];
			m_Reader.read(str, length);
			str[length] = 0;
			value = str;
			delete[] str;
		}
		return value;
	}

	std::string StreamReader::ReadString()
	{
		LIBSWBF2_LOG_INFO("StreamReader::ReadString()");
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

	void StreamReader::Close()
	{
		LIBSWBF2_LOG_INFO("StreamReader::Close()");
		if (!m_Reader.is_open()) {
			LIBSWBF2_THROW("Nothing has been opened yet");
		}

		m_FileName = "";
		m_Reader.close();
	}

	size_t StreamReader::GetPosition()
	{
		LIBSWBF2_LOG_INFO("StreamReader::GetPosition()");
		return (size_t)m_Reader.tellg();
	}

	void StreamReader::SetPosition(size_t NewPosition)
	{
		LIBSWBF2_LOG_INFO("StreamReader::SetPosition({})", NewPosition);
		if (NewPosition > m_FileSize)
		{
			LIBSWBF2_LOG_WARN("Cannot set read position to {:#x} because it is out of range! Range: 0x00 - {:#x}", NewPosition, m_FileSize);
			return;
		}

		m_Reader.seekg(NewPosition);
	}

	size_t StreamReader::GetFileSize()
	{
		LIBSWBF2_LOG_INFO("StreamReader::GetFileSize()");
		return m_FileSize;
	}

	bool StreamReader::CheckGood(size_t ReadSize)
	{
		LIBSWBF2_LOG_INFO("StreamReader::CheckGood({})", ReadSize);
		if (!m_Reader.is_open())
		{
			LIBSWBF2_THROW("Error during read process! File '{}' is not open", m_FileName);
		}

		if (!m_Reader.good())
		{
			std::string reason = "";
			if (m_Reader.eof())
			{
				reason += " End of File reached";
			}
			if (m_Reader.fail())
			{
				reason += " Logical Error on I/O operation";
			}
			if (m_Reader.bad())
			{
				reason += " Reading Error on I/O operation";
			}
			LIBSWBF2_THROW("Error during read process in '{}'! Reason: {}", m_FileName, reason);
		}

		size_t current = (size_t)m_Reader.tellg();
		if (current + ReadSize > m_FileSize)
		{
			LIBSWBF2_THROW("Reading {:#x} bytes will end up out of file!  Current position: {:#x}  FileSize: {:#x}", ReadSize, current, m_FileSize);
		}

		return true;
	}

	bool StreamReader::SkipBytes(size_t Amount)
	{
		LIBSWBF2_LOG_INFO("StreamReader::SkipBytes({})", Amount);
		if (CheckGood(Amount))
		{
			m_Reader.seekg(GetPosition() + Amount);
			return true;
		}
		return false;
	}
}

//#endif
