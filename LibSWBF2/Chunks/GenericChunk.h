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
		virtual ~GenericBaseChunk() override { }

		std::string GetHeaderName() const;
		GenericBaseChunk* GetParent() const;
		const std::vector<std::shared_ptr<GenericBaseChunk>>& GetChildren() const;

		virtual std::string ToString() const;

	public:
		template<class T>
		std::shared_ptr<T> ReadChild(FileReader &stream)
		{
			static_assert(std::is_base_of_v<GenericBaseChunk, T>);
			std::shared_ptr<T> chunk = std::make_shared<T>();
			m_Children.push_back(chunk);
			chunk->m_Parent = this;

			// Important: start reading AFTER parent and child have been set!
			chunk->ReadFromStream(stream);

			return chunk;
		}

		void ReadExplicitNoCtor(GenericBaseChunk *parent, FileReader &stream, std::shared_ptr<GenericBaseChunk> chunkPtr)
		{
			parent->m_Children.push_back(chunkPtr);
			chunkPtr->m_Parent = parent;

			// Important: start reading AFTER parent and child have been set!
			chunkPtr->ReadFromStream(stream);
		}

		void ReadGenerics(FileReader& stream);

	private:
		GenericBaseChunk *m_Parent;
		std::vector<std::shared_ptr<GenericBaseChunk>> m_Children;
	};

	struct LIBSWBF2_API GenericChunk : public GenericBaseChunk
	{
	protected:
		void Check(FileReader& stream);

	public:
		void RefreshSize() override;
		void WriteToStream(FileWriter& stream) override;
		void ReadFromStream(FileReader& stream) override;

		virtual uint32_t GetHeader() { return 0; }
	};
}
