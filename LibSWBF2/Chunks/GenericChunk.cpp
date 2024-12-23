#include "pch.h"
#include "GenericChunk.h"
#include "BaseChunk.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "Logging/Logger.h"

#include "STR.h"

#include "LVL/tex_/tex_.h"
#include "LVL/modl/LVL.modl.h"
#include "LVL/gmod/gmod.h"
#include "LVL/wrld/wrld.h"
#include "LVL/wrld/SIZE.h"
#include "LVL/wrld/XFRM.h"
#include "LVL/tern/tern.h"
#include "LVL/scr_/scr_.h"
#include "LVL/zaa_/zaa_.h"
#include "LVL/zaf_/zaf_.h"
#include "LVL/skel/skel.h"
#include "LVL/sound/_pad.h"
#include "LVL/sound/StreamList.h"
#include "LVL/sound/SoundBankList.h"
#include "LVL/sound/Stream.h"
#include "LVL/sound/SampleBank.h"
#include "LVL/Locl/Locl.h"
#include "LVL/lvl_.h"
#include "LVL/LVL.h"
#include "LVL/common/GenericClass.h"
#include "LVL/config/ConfigChunk.h"

#include "LVL/coll/coll.h"
#include "LVL/prim/prim.h"
#include "LVL/plan/plan.h"
#include "LVL/plan/plnp.h"


namespace LibSWBF2::Chunks
{
	void GenericChunk::Check(FileReader& stream)
	{
		// check for correct header
		ChunkHeader expected;
		expected.m_Magic = GetHeader();
		if (GetHeader() != 0 && m_Header != expected)
		{
			LIBSWBF2_THROW("{} expected '{}' but got '{}'", typeid(*this).name(), expected.ToString(), m_Header.ToString());
		}

		// check if current "chunk" exceeds parents data size
		GenericBaseChunk* parent = GetParent();
		if (parent != nullptr && (stream.GetPosition() + m_Size) > (parent->GetDataPosition() + parent->GetDataSize()))
		{
			LIBSWBF2_THROW("Current chunk size {:#x} exceeds parent data size!", m_Size);
		}
	}

	void GenericChunk::RefreshSize()
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void GenericChunk::WriteToStream(FileWriter& stream)
	{
		LIBSWBF2_THROW("Not implemented!");
	}

	void GenericBaseChunk::ReadGenerics(FileReader& stream)
	{
		while (ThereIsAnother(stream))
		{
			ChunkHeader nextHead = stream.ReadChunkHeader(true);
			if (IsKnownHeader(nextHead))
			{
				std::shared_ptr<GenericBaseChunk> chunk;
				try
				{
					// Special case: lvl_ might not be loaded!
					// See lvl_.cpp for more information
					if (nextHead == "lvl_"_h)
					{
						chunk = ReadChild<LVL::lvl_>(stream);
					}

					else if (nextHead == "gmod"_h)
					{
						chunk = ReadChild<LVL::gmod::gmod>(stream);
					}

					else if (nextHead == "NAME"_h)
					{
						chunk = ReadChild<STR<"NAME"_m>>(stream);
					}
					else if (nextHead == "PRNT"_h)
					{
						// TODO: is PRNT really always just a string?
						chunk = ReadChild<STR<"PRNT"_m>>(stream);
					}
					else if (nextHead == "tex_"_h)
					{
						chunk = ReadChild<LVL::texture::tex_>(stream);
					}
					else if (nextHead == "modl"_h)
					{
						chunk = ReadChild<LVL::modl::modl>(stream);
					}
					else if (nextHead == "skel"_h)
					{
						chunk = ReadChild<LVL::skel::skel>(stream);
					}
					else if (nextHead == "wrld"_h)
					{
						chunk = ReadChild<LVL::wrld::wrld>(stream);
					}
					else if (nextHead == "tern"_h)
					{
						chunk = ReadChild<LVL::terrain::tern>(stream);
					}
					else if (nextHead == "scr_"_h)
					{
						chunk = ReadChild<LVL::script::scr_>(stream);
					}
					else if (nextHead == "zaa_"_h)
					{
						chunk = ReadChild<LVL::animation::zaa_>(stream);
					}
					else if (nextHead == "zaf_"_h)
					{
						chunk = ReadChild<LVL::animation::zaf_>(stream);
					}
					else if (nextHead == "_pad"_h)
					{
						chunk = ReadChild<LVL::sound::_pad>(stream);
					}
					else if (nextHead == "lght"_h)
					{
						chunk = ReadChild<LVL::config::lght>(stream);
					}
					else if (nextHead == "sky_"_h)
					{
						chunk = ReadChild<LVL::config::sky_>(stream);
					}
					else if (nextHead == "fx__"_h)
					{
						chunk = ReadChild<LVL::config::fx__>(stream);
					}
					else if (nextHead == "bnd_"_h)
					{
						chunk = ReadChild<LVL::config::bnd_>(stream);
					}
					else if (nextHead == "prp_"_h)
					{
						chunk = ReadChild<LVL::config::prp_>(stream);
					}
					else if (nextHead == "path"_h)
					{
						chunk = ReadChild<LVL::config::path>(stream);
					}
					else if (nextHead == "comb"_h)
					{
						chunk = ReadChild<LVL::config::comb>(stream);
					}
					else if (nextHead == "snd_"_h)
					{
						chunk = ReadChild<LVL::config::snd_>(stream);
					}
					else if (nextHead == "mus_"_h)
					{
						chunk = ReadChild<LVL::config::mus_>(stream);
					}
					else if (nextHead == "ffx_"_h)
					{
						chunk = ReadChild<LVL::config::ffx_>(stream);
					}
					else if (nextHead == "tsr_"_h)
					{
						chunk = ReadChild<LVL::config::tsr_>(stream);
					}
					else if (nextHead == "hud_"_h)
					{
						chunk = ReadChild<LVL::config::hud_>(stream);
					}
					else if (nextHead == "Locl"_h)
					{
						chunk = ReadChild<LVL::Localization::Locl>(stream);
					}
					else if (nextHead == "entc"_h)
					{
						chunk = ReadChild<LVL::common::entc>(stream);
					}
					else if (nextHead == "ordc"_h)
					{
						chunk = ReadChild<LVL::common::ordc>(stream);
					}
					else if (nextHead == "wpnc"_h)
					{
						chunk = ReadChild<LVL::common::wpnc>(stream);
					}
					else if (nextHead == "expc"_h)
					{
						chunk = ReadChild<LVL::common::expc>(stream);
					}
					else if (nextHead == "coll"_h)
					{
						chunk = ReadChild<LVL::coll::coll>(stream);
					}					
					else if (nextHead == "prim"_h)
					{
						chunk = ReadChild<LVL::prim::prim>(stream);
					}
					else if (nextHead == "StreamList"_fnvh)
					{
						chunk = ReadChild<LVL::sound::StreamList>(stream);
					}
					else if (nextHead == "SoundBankList"_fnvh)
					{
						chunk = ReadChild<LVL::sound::SoundBankList>(stream);
					}
					else if (nextHead == "plan"_h)
					{
						chunk = ReadChild<plan::plan>(stream);
					}
					else if (nextHead == "plnp"_h)
					{
						chunk = ReadChild<plnp::plnp>(stream);
					}
					else if (nextHead == "Stream"_fnvh)
					{
						chunk = ReadChild<LVL::sound::Stream>(stream);
					}
					else if (nextHead == "SampleBank"_fnvh)
					{
						chunk = ReadChild<LVL::sound::SampleBank>(stream);
					}
					else
					{
						chunk = ReadChild<GenericChunk>(stream);
					}

					LIBSWBF2_LOG_INFO("Adding Child '{}' to '{}'", chunk->ToString(), m_Header.ToString());
				}
				catch (const LibSWBF2Exception &e)
				{
					LIBSWBF2_LOG_ERROR("{}", e.what());
					throw;
				}
			}
			else
			{
				stream.SkipBytes(4);
			}
		}
	}

	void GenericChunk::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);
		ReadGenerics(stream);
		BaseChunk::EnsureEnd(stream);
	}

	std::string GenericBaseChunk::GetHeaderName() const
	{
		return m_Header.ToString();
	}

	GenericBaseChunk* GenericBaseChunk::GetParent() const
	{
		return m_Parent;
	}

	const std::vector<std::shared_ptr<GenericBaseChunk>>& GenericBaseChunk::GetChildren() const
	{
		return m_Children;
	}

	std::string GenericBaseChunk::ToString() const
	{
		return "No Info";
	}
}
