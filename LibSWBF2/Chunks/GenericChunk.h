#pragma once
#include "BaseChunk.h"
#include "HeaderNames.h"
#include <string>
#include <vector>

namespace LibSWBF2::Chunks
{
	struct LIBSWBF2_API GenericBaseChunk : public BaseChunk
	{
		GenericBaseChunk() = default;
		virtual ~GenericBaseChunk() override;

		std::string GetHeaderName() const;
		GenericBaseChunk* GetParent() const;
		const std::vector<GenericBaseChunk*>& GetChildren() const;

		virtual std::string ToString() const;

	public:
		template<class ChildType>
		void ReadChildExplicit(GenericBaseChunk* parent, FileReader& stream, ChildType*& memberPtr)
		{
			ChildType* chunk = new ChildType();
			memberPtr = chunk;
			parent->m_Children.push_back(chunk);
			chunk->m_Parent = parent;

			// Important: start reading AFTER parent and child have been set!
			chunk->ReadFromStream(stream);
		}

		void ReadExplicitNoCtor(GenericBaseChunk* parent, FileReader& stream, GenericBaseChunk* chunkPtr)
		{
			parent->m_Children.push_back(chunkPtr);
			chunkPtr->m_Parent = parent;

			// Important: start reading AFTER parent and child have been set!
			chunkPtr->ReadFromStream(stream);
		}

		void ReadGenerics(FileReader& stream);

	private:
		GenericBaseChunk* m_Parent = nullptr;
		std::vector<GenericBaseChunk*> m_Children;
	};

	template<uint32_t Header>
	struct LIBSWBF2_API GenericChunk : public GenericBaseChunk
	{
    protected:
		void Check(FileReader& stream);

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;
	};

	// do not perform any checks on this one
	struct LIBSWBF2_API GenericChunkNC : public GenericChunk<0> {};


#define READ_CHILD(stream, member) ReadChildExplicit(this, stream, member);
#define READ_CHILD_GENERIC(stream) { GenericChunkNC* genericChild; READ_CHILD(stream, genericChild); }
}

