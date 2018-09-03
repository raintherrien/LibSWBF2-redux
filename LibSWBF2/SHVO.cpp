#include "stdafx.h"
#include "SHVO.h"

namespace LibSWBF2::Chunks::MSH
{
	SHVO::SHVO()
	{

	}

	SHVO::~SHVO()
	{

	}

	void SHVO::WriteToStream(ofstream& stream)
	{
		BaseChunk::WriteToStream(stream);
		stream << ShadowVolume;
	}

	void SHVO::ReadFromStream(ifstream& stream)
	{
		BaseChunk::ReadFromStream(stream);
		stream >> ShadowVolume;
	}
}