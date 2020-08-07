#include "stdafx.h"
#include "GenericChunk.h"
#include "BaseChunk.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "Logging/Logger.h"

#include "STR.h"

#include "LVL/tex_/tex_.h"
#include "LVL/modl/LVL.modl.h"
#include "LVL/wrld/wrld.h"
#include "LVL/tern/tern.h"
#include "LVL/scr_/scr_.h"
#include "LVL/zaa_/zaa_.h"
#include "LVL/skel/skel.h"
#include "LVL/sound/emo_.h"
#include "LVL/sound/_pad.h"
#include "LVL/lvl_.h"
#include "LVL/LVL.h"


namespace LibSWBF2::Chunks
{
	GenericBaseChunk::~GenericBaseChunk()
	{
		for (size_t i = 0; i < m_Children.Size(); ++i)
		{
			delete m_Children[i];
		}
	}

	template<uint32_t Header>
	void GenericChunk<Header>::Check(FileReader& stream)
	{
		// check for correct header
		ChunkHeader expected;
		expected.m_Magic = Header;
		if (Header != 0 && m_Header != expected)
		{
			THROW("Expected '{}' but got '{}'", expected, m_Header);
		}

		// check if current "chunk" exceeds parents data size
		GenericBaseChunk* parent = GetParent();
		if (parent != nullptr && (stream.GetPosition() + m_Size) > (parent->GetDataPosition() + parent->GetDataSize()))
		{
			THROW("Current chunk size {} exceeds parent data size!", m_Size);
		}
	}

	template<uint32_t Header>
	void GenericChunk<Header>::RefreshSize()
	{
		THROW("Not implemented!");
	}

	template<uint32_t Header>
	void GenericChunk<Header>::WriteToStream(FileWriter& stream)
	{
		THROW("Not implemented!");
	}

	void GenericBaseChunk::ReadGenerics(FileReader& stream)
	{
		while (ThereIsAnother(stream))
		{
			ChunkHeader nextHead = stream.ReadChunkHeader(true);
			if (IsKnownHeader(nextHead))
			//if (IsValidHeader(nextHead))
			{
				GenericBaseChunk* chunk = nullptr;
				try
				{
					// Special case: lvl_ might not be loaded!
					// See lvl_.cpp for more information
					if (nextHead == "lvl_"_h)
					{
						LVL::lvl_* subLVL;
						READ_CHILD(stream, subLVL);
						chunk = subLVL;
					}
					else if (nextHead == "NAME"_h)
					{
						STR<"NAME"_m>* name;
						READ_CHILD(stream, name);
						chunk = name;
					}
					else if (nextHead == "PRNT"_h)
					{
						// TODO: is PRNT really always just a string?
						STR<"PRNT"_m>* name;
						READ_CHILD(stream, name);
						chunk = name;
					}
					else if (nextHead == "tex_"_h)
					{
						LVL::texture::tex_* name;
						READ_CHILD(stream, name);
						chunk = name;
					}
					else if (nextHead == "modl"_h)
					{
						LVL::modl::modl* model;
						READ_CHILD(stream, model);
						chunk = model;
					}
					else if (nextHead == "skel"_h)
					{
						LVL::skel::skel* skeleton;
						READ_CHILD(stream, skeleton);
						chunk = skeleton;
					}
					else if (nextHead == "wrld"_h)
					{
						LVL::wrld::wrld* world;
						READ_CHILD(stream, world);
						chunk = world;
					}
					else if (nextHead == "tern"_h)
					{
						LVL::terrain::tern* terrain;
						READ_CHILD(stream, terrain);
						chunk = terrain;
					}
					else if (nextHead == "scr_"_h)
					{
						LVL::script::scr_* script;
						READ_CHILD(stream, script);
						chunk = script;
					}
					else if (nextHead == "zaa_"_h)
					{
						LVL::animation::zaa_* zaabin;
						READ_CHILD(stream, zaabin);
						chunk = zaabin;
					}
					else if (nextHead == "emo_"_h)
					{
						LVL::sound::emo_* unknown;
						READ_CHILD(stream, unknown);
						chunk = unknown;
					}
					else if (nextHead == "_pad"_h)
					{
						LVL::sound::_pad* unknown;
						READ_CHILD(stream, unknown);
						chunk = unknown;
					}
					else
					{
						GenericChunkNC* generic;
						READ_CHILD(stream, generic);
						chunk = generic;
					}

					LOG_INFO("Adding Child '{}' to '{}'", chunk->GetHeader(), m_Header);
				}
				catch (LibException& e)
				{
					delete chunk;
					chunk = nullptr;

					LOG_ERROR(e.what());
					//LOG_ERROR("Skipping invalid Chunk: '{}' at pos: {}", nextHead, stream.GetPosition() - 8);
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	template<uint32_t Header>
	void GenericChunk<Header>::ReadFromStream(FileReader& stream)
	{
		BaseChunk::ReadFromStream(stream);
		Check(stream);
		ReadGenerics(stream);
		BaseChunk::EnsureEnd(stream);
	}

	String GenericBaseChunk::GetHeaderName() const
	{
		return m_Header.ToString();
	}

	GenericBaseChunk* GenericBaseChunk::GetParent() const
	{
		return m_Parent;
	}

	const List<GenericBaseChunk*>& GenericBaseChunk::GetChildren() const
	{
		return m_Children;
	}

	String GenericBaseChunk::ToString()
	{
		return "No Info";
	}
}

// ============================================================
// ============== Explicit Chunk instantiations ===============
// ============================================================

namespace LibSWBF2::Chunks
{
	template struct LIBSWBF2_API GenericChunk<0>;
	template struct LIBSWBF2_API GenericChunk<"ucfb"_m>;
	template struct LIBSWBF2_API GenericChunk<"LVL_"_m>;
	template struct LIBSWBF2_API GenericChunk<"INFO"_m>;
	template struct LIBSWBF2_API GenericChunk<"TYPE"_m>;
	template struct LIBSWBF2_API GenericChunk<"BODY"_m>;
	template struct LIBSWBF2_API GenericChunk<"FACE"_m>;
	template struct LIBSWBF2_API GenericChunk<"FMT_"_m>;
	template struct LIBSWBF2_API GenericChunk<"tex_"_m>;
	template struct LIBSWBF2_API GenericChunk<"modl"_m>;
	template struct LIBSWBF2_API GenericChunk<"segm"_m>;
	template struct LIBSWBF2_API GenericChunk<"MTRL"_m>;
	template struct LIBSWBF2_API GenericChunk<"tern"_m>;
	template struct LIBSWBF2_API GenericChunk<"PCHS"_m>;
	template struct LIBSWBF2_API GenericChunk<"PTCH"_m>;
	template struct LIBSWBF2_API GenericChunk<"VBUF"_m>;
	template struct LIBSWBF2_API GenericChunk<"IBUF"_m>;
	template struct LIBSWBF2_API GenericChunk<"LTEX"_m>;
	template struct LIBSWBF2_API GenericChunk<"scr_"_m>;
	template struct LIBSWBF2_API GenericChunk<"zaa_"_m>;
	template struct LIBSWBF2_API GenericChunk<"BIN_"_m>;
	template struct LIBSWBF2_API GenericChunk<"SMNA_"_m>;
	template struct LIBSWBF2_API GenericChunk<"skel"_m>;
	template struct LIBSWBF2_API GenericChunk<"SKIN"_m>;
	template struct LIBSWBF2_API GenericChunk<"BMAP"_m>;
	template struct LIBSWBF2_API GenericChunk<"emo_"_m>;
	template struct LIBSWBF2_API GenericChunk<"_pad"_m>;
	template struct LIBSWBF2_API GenericChunk<"XFRM"_m>;
	template struct LIBSWBF2_API GenericChunk<"inst"_m>;
	template struct LIBSWBF2_API GenericChunk<"DTEX"_m>;

	// string chunks (see STR.cpp)
	template struct LIBSWBF2_API GenericChunk<"NAME"_m>;
	template struct LIBSWBF2_API GenericChunk<"TNAM"_m>;
	template struct LIBSWBF2_API GenericChunk<"SNAM"_m>;
	template struct LIBSWBF2_API GenericChunk<"TX0D"_m>;
	template struct LIBSWBF2_API GenericChunk<"TX1D"_m>;
	template struct LIBSWBF2_API GenericChunk<"TX2D"_m>;
	template struct LIBSWBF2_API GenericChunk<"TX3D"_m>;
	template struct LIBSWBF2_API GenericChunk<"PRNT"_m>;
	template struct LIBSWBF2_API GenericChunk<"NODE"_m>;
	template struct LIBSWBF2_API GenericChunk<"RTYP"_m>;
	template struct LIBSWBF2_API GenericChunk<"BNAM"_m>;
	template struct LIBSWBF2_API GenericChunk<"DTLX"_m>;
}