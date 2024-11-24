#include "pch.h"
#include "SoundBankList.h"
#include "SampleBank.h"
#include "FileReader.h"
#include "InternalHelpers.h"
#include "Hashing.h"


namespace LibSWBF2::Chunks::LVL::sound
{
	void SoundBankList::RefreshSize()
	{
		THROW("Not implemented!");
	}

	void SoundBankList::WriteToStream(FileWriter& stream)
	{
		THROW("Not implemented!");
	}

	void SoundBankList::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		while (ThereIsAnother(stream))
		{
			ChunkHeader next = stream.ReadChunkHeader(true);
			if (next == "SampleBank"_fnvh)
			{
				READ_CHILD(stream, m_SampleBanks.emplace_back())
			}
			else 
			{
				stream.SkipBytes(4);
			}		
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string SoundBankList::ToString() const
	{
		std::string result = fmt::format(
			"Num sample banks: {0}",
			m_SampleBanks.size()
		);

		return result;
	}
}
