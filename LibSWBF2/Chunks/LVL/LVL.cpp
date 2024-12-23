#include "pch.h"
#include "LVL.h"
#include "InternalHelpers.h"
#include "Hashing.h"
#include "FileReader.h"
#include "Chunks/HeaderNames.h"

#include "Chunks/LVL/sound/Stream.h"
#include "Chunks/LVL/sound/StreamInfo.h"

#include <sstream>
#include <iostream>

namespace LibSWBF2::Chunks::LVL
{
	using LibSWBF2::Chunks::LVL::sound::Stream;

	void LVL::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);
		ReadGenerics(stream);
		BaseChunk::EnsureEnd(stream);
	}


	std::shared_ptr<Stream> LVL::FindAndReadSoundStream(FileReader& stream, FNVHash soundStreamName)
	{
		// First check if we already loaded the stream...
		const std::vector<std::shared_ptr<GenericBaseChunk>>& children = GetChildren();
		for (int i = 0; i < children.size(); i++)
		{
			std::shared_ptr<Stream> streamChunk = std::dynamic_pointer_cast<Stream>(children[i]);
			if (streamChunk && streamChunk->p_Info != nullptr && streamChunk->p_Info->m_Name == soundStreamName) {
				return streamChunk;
			}
		}

		stream.SetPosition(GetDataPosition());

		while (ThereIsAnother(stream))
		{
			ChunkHeader nextHead = stream.ReadChunkHeader(true);
			if (IsKnownHeader(nextHead))
			{
				//try
				//{
					if (nextHead == "StreamList"_fnvh || nextHead == "lvl_"_h || nextHead == "_pad"_h)
					{
						stream.SkipBytes(8);
					}
					else if (nextHead == "Stream"_fnvh && Stream::PeekStreamName(stream) == soundStreamName)
					{
						return ReadChild<Stream>(stream);
					}
					else 
					{
						BaseChunk::SkipChunk(stream,false);
					}
				//}
				//catch (const LibSWBF2Exception &e)
				//{
				//	LIBSWBF2_LOG_WARN("{}", e.what());
				//	return {};
				//}
			}
			else 
			{
				stream.SkipBytes(4);
			}
		}

		return {};
	}




	bool LVL::ReadFile(std::string Path, const std::vector<std::string>* subLVLsToLoad)
	{
		m_SubLVLsToLoad.clear();

		// Loading a LVL file can be accompanied by additional parameters.
		// These parameters usually describe what sub LVLs to load (at least
		// I did not found no other purpose as I'm writing this), all other
		// sub LVL will be ignored.
		// If not a single parameter is specified, currently ALL sub LVL are loaded!
		if (subLVLsToLoad != nullptr)
		{
			for (size_t i = 0; i < subLVLsToLoad->size(); ++i)
			{
				m_SubLVLsToLoad.push_back(FNV::Hash((*subLVLsToLoad)[i]));
			}
		}
		
		// There seems to be another pattern to be used aswell, where
		// parameters are just concatenated to the path using ';' as delimiter
		std::vector<std::string> params;
		std::istringstream stream(Path);
		std::string nextParam;
		for (bool first = true; std::getline(stream, nextParam, ';'); first = false)
		{
			if (first)
			{
				// first parameter is the actual path
				Path = nextParam.c_str();
			}
			else
			{
				// arguments
				params.push_back(nextParam);
			}
		}

		if (params.size() > 0)
		{
			for (std::string param : params)
			{
				m_SubLVLsToLoad.push_back(FNV::Hash(param.c_str()));
			}
		}

		// Debug purposes only. Should be removed at some point
		if (params.size() > 1)
		{
			LIBSWBF2_LOG_WARN("LVL Path contains more than one parameter! This is not expected... Research is needed!");

			std::string str;
			for (std::string param : params)
			{
				str += "\t" + param + "\n";
			}
			LIBSWBF2_LOG_WARN("LVL Path parameters are: \n{}", str);
		}

		bool success = BaseChunk::ReadFromFile(Path);
		m_SubLVLsToLoad.clear();
		return success;
	}
}
