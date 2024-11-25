#include "pch.h"
#include "Vector2.h"
#include "FileWriter.h"
#include "FileReader.h"
#include <fmt/format.h>

namespace LibSWBF2::Types
{
	Vector2::Vector2() : m_X(0.0f), m_Y(0.0f) 
	{
	}

	Vector2::Vector2(const float_t x, const float_t y)
	{
		m_X = x;
		m_Y = y;
	}

	Vector2::~Vector2()
	{

	}

	void Vector2::WriteToStream(FileWriter& stream)
	{
		stream.WriteFloat(m_X);
		stream.WriteFloat(m_Y);
	}

	void Vector2::ReadFromStream(FileReader& stream)
	{
		m_X = stream.ReadFloat();
		m_Y = stream.ReadFloat();
	}

	std::string Vector2::ToString() const
	{
		return fmt::format("[{}, {}]", (float_t) m_X, (float_t) m_Y);
	}
}
