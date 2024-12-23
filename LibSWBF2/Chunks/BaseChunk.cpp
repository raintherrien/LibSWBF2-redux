#include "pch.h"
#include "BaseChunk.h"
#include "InternalHelpers.h"
#include "FileWriter.h"
#include "FileReader.h"
#include "Logging/Logger.h"
#include <string>

#include "MemoryMappedReader.h"

namespace LibSWBF2::Chunks
{
	void BaseChunk::RefreshSize()
	{
		// must be overwritten by inheriting classes!
	}

	void BaseChunk::WriteToStream(FileWriter& stream)
	{
		RefreshSize();
		stream.WriteChunkHeader(m_Header);
		stream.WriteChunkSize(m_Size);
	}

	void BaseChunk::ReadFromStream(FileReader& stream)
	{
		m_ChunkPosition = stream.GetPosition();
		m_Header = stream.ReadChunkHeader(false);
		m_Size = stream.ReadChunkSize();

		LIBSWBF2_LOG_DEBUG("Position: {}", m_ChunkPosition);
		LIBSWBF2_LOG_DEBUG("Header: {}", m_Header.ToString());
		LIBSWBF2_LOG_DEBUG("Size: {:#x}", m_Size);

		if (stream.GetPosition() + m_Size > 8 + stream.GetFileSize())
		{
			LIBSWBF2_THROW("Chunk is too big and will end up out of file! Chunk: '{}' Size: {:#x} At Position: {:#x} with File Size of: {:#x} (File: {})", m_Header.ToString(), m_Size, stream.GetPosition() - 8, stream.GetFileSize(), stream.GetFileName());
		}
	}

	bool BaseChunk::WriteToFile(const std::string &Path)
	{
		FileWriter writer;
		if (writer.Open(Path))
		{
			try
			{
				WriteToStream(writer);
				writer.Close();
			}
			catch (const LibSWBF2Exception &e)
			{
				LIBSWBF2_LOG_ERROR("{}", e.what());
				LIBSWBF2_LOG_ERROR("Aborting write process...");
				throw;
			}
			LIBSWBF2_LOG_INFO("Successfully finished writing process!");
			return true;
		}
		LIBSWBF2_LOG_WARN("Could not write to File {}!", Path);
		return false;
	}

	bool BaseChunk::ReadFromFile(const std::string &Path)
	{
		MemoryMappedReader reader;
		if (reader.Open(Path))
		{
			try
			{
				ReadFromStream(reader);
				reader.Close();
				LIBSWBF2_LOG_INFO("Successfully finished reading process!");
				return true;
			}
			catch (const LibSWBF2Exception &e)
			{
				LIBSWBF2_LOG_ERROR("{}", e.what());
				LIBSWBF2_LOG_ERROR("Aborting read process...");
				reader.Close();
				throw;
			}
		}
		else
		{
			LIBSWBF2_LOG_ERROR("Could not open File {}! Non existent?", Path);
		}

		return false;
	}

	ChunkHeader BaseChunk::GetHeader() const
	{
		return m_Header;
	}

	ChunkSize BaseChunk::GetDataSize() const
	{
		return m_Size;
	}

	ChunkSize BaseChunk::GetAlignedSize() const
	{
		return (ChunkSize)ceil(m_Size / 4.0f) * 4;
	}

	ChunkSize BaseChunk::GetFullSize() const
	{
		return GetAlignedSize() + sizeof(ChunkHeader) + sizeof(ChunkSize);
	}

	size_t BaseChunk::GetPosition() const
	{
		return m_ChunkPosition;
	}

	size_t BaseChunk::GetDataPosition() const
	{
		return m_ChunkPosition + sizeof(ChunkHeader) + sizeof(ChunkSize);
	}

	bool BaseChunk::PositionInChunk(const size_t& PossiblePosition)
	{
		size_t dataPosition = GetDataPosition();
		return PossiblePosition >= dataPosition && PossiblePosition < dataPosition + m_Size;
	}

	bool BaseChunk::ThereIsAnother(FileReader& stream)
	{
		return stream.GetFileSize() - stream.GetPosition() >= 8 && PositionInChunk(stream.GetPosition() + 7);
	}

	bool BaseChunk::SkipChunk(FileReader& stream, const bool& printWarn)
	{
		if (stream.GetPosition() == stream.GetFileSize())
		{
			LIBSWBF2_LOG_WARN("Cannot skip chunk from end position: {:#x}", stream.GetPosition());
			return false;
		}

		ChunkHeader head = stream.ReadChunkHeader(false);
		ChunkSize alignedSize = (ChunkSize)ceil(stream.ReadChunkSize() / 4.0f) * 4;

		if (printWarn)
		{
			LIBSWBF2_LOG_WARN("[{}] Unexpected Chunk found: {} at position {:#x}. Skipping {:#x} Bytes...", m_Header.ToString(), head.ToString(), stream.GetPosition(), alignedSize);
		}

		return stream.SkipBytes(alignedSize);
	}

	void BaseChunk::EnsureEnd(FileReader& stream)
	{
		size_t currPos = stream.GetPosition();
		if (currPos == stream.GetFileSize())
			return;

		size_t endPos = GetDataPosition() + GetAlignedSize();
		if (currPos < endPos)
		{
			//LIBSWBF2_LOG_WARN("[{}] We did not end up at the Chunks end position ({:#x})! Instead we are here:{:#x}! Moving Position to Chunks end position...", m_Header, endPos, stream.GetPosition());
			stream.SetPosition(endPos >= stream.GetFileSize() ? stream.GetFileSize() - 1 : endPos);
		}
		else if (currPos > endPos)
		{
			// This should NEVER happen!
			LIBSWBF2_LOG_WARN("[{}] Ended up outside of current chunk (end is at: {:#x}, we are at: {:#x}! Too many bytes read: {} (File: {})", m_Header.ToString(), endPos, currPos, currPos - endPos, stream.GetFileName());
			ForwardToNextHeader(stream);
		}
	}

	void BaseChunk::ForwardToNextHeader(FileReader& stream)
	{
		// if we've got any trailing bytes, skip them too
		size_t lastPos = stream.GetPosition();
		while (stream.GetFileSize() - stream.GetPosition() >= 4 && !IsKnownHeader(stream.ReadChunkHeader(true)))
		{
			stream.SetPosition(stream.GetPosition() + 1);
		}
		size_t skipped = stream.GetPosition() - lastPos;
		LIBSWBF2_LOG_WARN("[{}] Forwarded to next header at {:#x}, skipped {} bytes!", m_Header.ToString(), stream.GetPosition(), skipped);
	}

	std::string BaseChunk::ToString()
	{
		return GetHeader().ToString();
	}
}
