#pragma once
#include "req.h"
#include <vector>

namespace LibSWBF2::Types
{
	#pragma pack(push,1)
	template<typename T>
	struct LIBSWBF2_API Key 
	{
		T time;
		float_t value;
	};
	#pragma pack(pop)


	template<typename T>
	struct LIBSWBF2_API Curve
	{
		const std::vector<T>& GetIndices() const
		{
			return m_Indices;
		}

		const std::vector<float_t>& GetValues() const
		{
			return m_Values;
		}

		const std::vector<Key<T>> GetKeys() const
		{
			const size_t& iSize = m_Indices.Size();
			const size_t& vSize = m_Values.Size();

			uint32_t size = iSize < vSize ? iSize : vSize;

			std::vector<Key<T>> keys;
			keys.Resize(size);

			for (uint16_t i = 0; i < size; i++)
			{
				keys.Add({ m_Indices[i], m_Values[i] });
			}

			return keys;
		}

		Curve(std::vector<T>&& inds, std::vector<float_t>&& vals)
		{
			m_Indices = inds;
			m_Values = vals;
		}

		Curve() = default;


	private:
		std::vector<T> m_Indices;
		std::vector<float_t> m_Values; 
	};
}
