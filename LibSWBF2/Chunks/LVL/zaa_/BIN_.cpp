#include "pch.h"
#include "BIN_.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::animation
{
	void BIN_::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void BIN_::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void BIN_::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		//Skip SMNA header and size
		stream.SkipBytes(8);

		m_Version = stream.ReadUInt32();
		m_NumBones = stream.ReadUInt32();
		m_DataBufferLength = stream.ReadUInt32();

		m_NumAnimations = stream.ReadUInt16();
		m_DebugLevel = stream.ReadUInt16();

		if (m_DebugLevel != 0 || m_Version != 8)
		{
			LIBSWBF2_LOG_ERROR("Encountered zaa_(BIN_) chunk with unusual version ({}) or debug level ({})...", m_Version, m_DebugLevel);

			while (ThereIsAnother(stream))
			{
				ReadChild<GenericChunk>(stream);
			}			
		}
		else
		{
			p_AnimsMetadata = ReadChild<MINA>(stream);
			p_JointAddresses = ReadChild<TNJA>(stream);
			p_CompressedAnimData = ReadChild<TADA>(stream);
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string BIN_::ToString() const
	{
		return fmt::format(
			"Chunk version: {}\n"
			"Num bones: {}\n"
			"Compressed anim data length: {}\n"
			"Num Animations: {}\n"
			"Debug Level: {}",
			m_Version,
			m_NumBones,
			m_DataBufferLength,
			m_NumAnimations,
			m_DebugLevel
		);
	}
}
