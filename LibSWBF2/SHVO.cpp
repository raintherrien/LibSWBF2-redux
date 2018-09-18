#include "stdafx.h"
#include "SHVO.h"

namespace LibSWBF2::Chunks::Mesh
{
	SHVO::SHVO()
	{

	}

	SHVO::~SHVO()
	{

	}

	void SHVO::RefreshSize()
	{
		m_Size = sizeof(int32_t);
	}

	void SHVO::WriteToStream(FileWriter& stream)
	{
		BaseChunk::WriteToStream(stream);
		stream.WriteInt32(m_ShadowVolume);
	}

	void SHVO::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		m_ShadowVolume = stream.ReadInt32();
	}

	int32_t SHVO::GetShadowVolume()
	{
		return m_ShadowVolume;
	}

	void SHVO::SetShadowVolume(int32_t ShadowVolume)
	{
		m_ShadowVolume = ShadowVolume;
	}
}