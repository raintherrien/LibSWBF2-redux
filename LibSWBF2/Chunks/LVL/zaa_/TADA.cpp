#include "pch.h"
#include "TADA.h"
#include "BIN_.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::animation
{
	void TADA::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void TADA::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}


	void TADA::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);

		//TADA size is usually incorrect, but correctly listed in BIN_
		m_Size = dynamic_cast<BIN_*>(GetParent()) -> m_DataBufferLength;

		Check(stream);

		//Keep quantized untill needed
		m_DataBufferLength = m_Size;
		p_DataBuffer = new uint8_t[m_DataBufferLength];
		stream.ReadBytes(p_DataBuffer, m_DataBufferLength);

		BaseChunk::EnsureEnd(stream);
	}

	std::string TADA::ToString() const
	{
		return fmt::format("Buffer length = {}", m_DataBufferLength);
	}

	TADA::~TADA()
	{
		delete p_DataBuffer;
	}
}
