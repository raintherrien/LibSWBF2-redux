#include "pch.h"
#include "FileWriter.h"
#include "InternalHelpers.h"
#include "InternalHelpers.h"
#include <string>

namespace LibSWBF2
{
	using LibSWBF2::Logging::Logger;

	bool FileWriter::Open(const std::string& File)
	{
		return Open(File, false);
	}

	bool FileWriter::Open(const std::string& File, const bool& LogFile)
	{
		m_Writer.open(File, std::ofstream::out | (LogFile ? std::ofstream::app : std::ofstream::binary | std::ofstream::trunc));
		bool success = m_Writer.good() && m_Writer.is_open();

		if (!success)
		{
			if (!LogFile)
			{
				LIBSWBF2_LOG_ERROR("File '{}' could not be found / created!", File);
			}
			m_FileName = "";
			m_Writer.close();
			return false;
		}

		m_FileName = File;

		if (!LogFile)
		{
			LIBSWBF2_LOG_INFO("File '{}' successfully created/opened.", m_FileName);
		}
		return true;
	}

	void FileWriter::WriteChunkHeader(const ChunkHeader& value)
	{
		if (CheckGood())
		{
			m_Writer.write((char*)&value, sizeof(value));
		}
	}

	void FileWriter::WriteChunkSize(ChunkSize value)
	{
		if (CheckGood())
		{
			m_Writer.write((char*)&value, sizeof(value));
		}
	}

	void FileWriter::WriteByte(uint8_t value)
	{
		if (CheckGood())
		{
			m_Writer.write((char*)&value, sizeof(value));
		}
	}

	void FileWriter::WriteBytes(uint8_t* data, size_t size)
	{
		if (CheckGood())
		{
			m_Writer.write((const char*)data, size);
		}
	}

	void FileWriter::WriteInt32(int32_t value)
	{
		if (CheckGood())
		{
			m_Writer.write((char*)&value, sizeof(value));
		}
	}

	void FileWriter::WriteUInt32(uint32_t value)
	{
		if (CheckGood())
		{
			m_Writer.write((char*)&value, sizeof(value));
		}
	}

	void FileWriter::WriteUInt16(uint16_t value)
	{
		if (CheckGood())
		{
			m_Writer.write((char*)&value, sizeof(value));
		}
	}

	void FileWriter::WriteFloat(float_t value)
	{
		if (CheckGood())
		{
			m_Writer.write((char*)&value, sizeof(value));
		}
	}

	void FileWriter::WriteString(const std::string& value)
	{
		// string in chunk needs to be a zero terminated c-string
		// size must be a multiple of 4
		// remaining bytes should be filled with 0x00

		if (CheckGood())
		{
			size_t length = value.size();
			m_Writer.write(value.c_str(), length);

			int remaining = 4 - (length % 4);
			char* empty = new char[remaining];

			for (int i = 0; i < remaining; i++)
				empty[i] = 0;

			m_Writer.write(empty, remaining);
			delete[] empty;
		}
	}

	void FileWriter::WriteString(const std::string& value, uint16_t fixedSize)
	{
		if (CheckGood())
		{
			if (value.size() > fixedSize)
			{
				LIBSWBF2_LOG_WARN("Actual string size ({}) is greater than fixed size ({}) !", value.size(), fixedSize);
			}

			m_Writer.write(value.c_str(), fixedSize);
		}
	}

	void FileWriter::WriteLine(const std::string& line)
	{
		if (CheckGood())
		{
			m_Writer << line << "\n";
			m_Writer.flush();
		}
	}

	void FileWriter::Close()
	{
		if (!m_Writer.is_open())
		{
			LIBSWBF2_LOG_WARN("Nothing has been opened yet!");
			return;
		}

		m_FileName = "";
		m_Writer.close();
	}

	bool FileWriter::CheckGood()
	{
		if (!m_Writer.is_open())
		{
			LIBSWBF2_THROW("Error during write process! File '{}' is not open!", m_FileName);
		}

		if (!m_Writer.good())
		{
			std::string reason = "";
			if (m_Writer.eof())
			{
				reason += " End of File reached!";
			}
			if (m_Writer.fail())
			{
				reason += " Logical Error on I/O operation!";
			}
			if (m_Writer.bad())
			{
				reason += " Writing Error on I/O operation!";
			}
			LIBSWBF2_THROW("Error during write process in '{}'! Reason: {}", m_FileName, reason);
		}

		return true;
	}

	size_t FileWriter::GetPosition()
	{
		return (size_t)m_Writer.tellp();
	}
}
