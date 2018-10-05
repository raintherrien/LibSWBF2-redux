#include "stdafx.h"
#include "MTYP.h"

namespace LibSWBF2::Chunks::Mesh
{
	MTYP::MTYP()
	{

	}

	MTYP::~MTYP()
	{

	}

	void MTYP::RefreshSize()
	{
		m_Size = sizeof(uint32_t);
	}

	void MTYP::WriteToStream(FileWriter& stream)
	{
		BaseChunk::WriteToStream(stream);
		stream.WriteUInt32(m_ModelType);
	}

	void MTYP::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		m_ModelType = (EModelType)stream.ReadUInt32();
		BaseChunk::EnsureEnd(stream);
	}
}