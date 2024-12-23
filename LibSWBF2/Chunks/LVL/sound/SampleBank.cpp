#include "pch.h"
#include "SampleBank.h"
#include "SampleBankInfo.h"
#include "SampleBankData.h"
#include "InternalHelpers.h"
#include "FileReader.h"


#include <iostream>

namespace LibSWBF2::Chunks::LVL::sound
{
	void SampleBank::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void SampleBank::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void SampleBank::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		while (ThereIsAnother(stream))
		{
			ChunkHeader next = stream.ReadChunkHeader(true);
			if (next == "Info"_fnvh)
			{
				p_Info = ReadChild<SampleBankInfo>(stream);
			}
			else if (next == "Data"_fnvh)
			{
				p_Data = ReadChild<SampleBankData>(stream);
			}
			else 
			{
				stream.SkipBytes(4);
			}
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string SampleBank::ToString() const
	{
		return p_Info == nullptr ? "No info chunk found" : p_Info -> HeaderToString();
	}
}
