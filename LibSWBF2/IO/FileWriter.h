#pragma once
#include <fstream>
#include <string>

namespace LibSWBF2
{
	union ChunkHeader;

	class FileWriter
	{
	public:
		bool Open(const std::string& File);
		bool Open(const std::string& File, const bool& LogFile);
		void WriteChunkHeader(const ChunkHeader& value);
		void WriteChunkSize(ChunkSize value);
		void WriteByte(uint8_t value);
		void WriteBytes(uint8_t* data, size_t size);
		void WriteInt32(int32_t value);
		void WriteUInt32(uint32_t value);
		void WriteUInt16(uint16_t value);
		void WriteFloat(float_t value);
		void WriteString(const std::string& value);
		void WriteString(const std::string& value, uint16_t fixedSize);
		void WriteLine(const std::string& line);
		size_t GetPosition();
		void Close();
	private:
		bool CheckGood();

		std::string m_FileName;
		std::ofstream m_Writer;
	};
}
