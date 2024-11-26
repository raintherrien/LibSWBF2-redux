#include "pch.h"
#include "zaa_.h"
#include "InternalHelpers.h"
#include "FileReader.h"

namespace LibSWBF2::Chunks::LVL::animation
{
	void TNJA::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void TNJA::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void TNJA::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);

		MINA *animData = dynamic_cast<BIN_*>(GetParent()) -> p_AnimsMetadata;

		if (animData == nullptr)
		{
			BaseChunk::EnsureEnd(stream);
			return;
		}

		for (int i = 0; i < animData -> m_AnimNameHashes.size(); i++)
		{
			for (int j = 0; j < animData -> m_AnimBoneCounts[i]; j++)
			{
				m_BoneCRCs.push_back(stream.ReadUInt32());

				for (int k = 0; k < 11; k++)
				{
					if (k < 4)
					{
						m_RotationOffsets.push_back(stream.ReadUInt32());
					}
					else if (k < 7)
					{
						m_TranslationOffsets.push_back(stream.ReadUInt32());
					}
					else
					{
						m_TranslationParams.push_back(stream.ReadFloat());
					}
				}				
			}
		}

		BaseChunk::EnsureEnd(stream);
	}

	std::string TNJA::ToString() const
	{
		BIN_ *parent = dynamic_cast<BIN_*>(GetParent());

		if (parent != nullptr)
		{
			MINA *metadata = parent -> p_AnimsMetadata;
			if (metadata != nullptr)
			{
				//Detailed summary
				std::string fancy = "";

				auto nameCRCs = metadata -> m_AnimNameHashes;

				int rotOffsetI = 0;
				int locOffsetI = 0;
				int locParamsI = 0;

				for (int i = 0; i < nameCRCs.size(); i++)
				{
					fancy += fmt::format("\nAnim 0x{0:x} decompression parameters: \n", nameCRCs[i]);

					for (int j = 0; j < metadata -> m_AnimBoneCounts[i]; j++)
					{
						fancy += fmt::format("\tBone 0x{0:x}: \n", m_BoneCRCs[j]);
						
						fancy += "\t\tComponent TADA offsets: \n\n";

						fancy += fmt::format("\t\t\tRot x: {}\n", m_RotationOffsets[rotOffsetI++]);
						fancy += fmt::format("\t\t\tRot y: {}\n", m_RotationOffsets[rotOffsetI++]);
						fancy += fmt::format("\t\t\tRot z: {}\n", m_RotationOffsets[rotOffsetI++]);
						fancy += fmt::format("\t\t\tRot w: {}\n\n", m_RotationOffsets[rotOffsetI++]);

						fancy += fmt::format("\t\t\tLoc x: {}\n", m_TranslationOffsets[locOffsetI++]);
						fancy += fmt::format("\t\t\tLoc y: {}\n", m_TranslationOffsets[locOffsetI++]);
						fancy += fmt::format("\t\t\tLoc z: {}\n\n", m_TranslationOffsets[locOffsetI++]);

						fancy += "\t\tLocation component dequantization params: \n";

						fancy += fmt::format("\t\t\tLoc x bias: {}\n", m_TranslationParams[locParamsI++]);
						fancy += fmt::format("\t\t\tLoc y bias: {}\n", m_TranslationParams[locParamsI++]);
						fancy += fmt::format("\t\t\tLoc z bias: {}\n", m_TranslationParams[locParamsI++]);
						fancy += fmt::format("\t\t\tLoc multiplier: {}\n", m_TranslationParams[locParamsI++]);
					}
				}

				return fancy;
			}
		}

		//Basic summary
		return fmt::format(
			"Num bone CRCs: {}\n",
			m_BoneCRCs.size()
		);
	}
}
