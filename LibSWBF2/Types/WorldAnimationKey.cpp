#include "pch.h"
#include "FileReader.h"
#include "FileWriter.h"
#include <fmt/format.h>
#include "WorldAnimationKey.h"
#include "Vector3.h"
#include <string>

namespace LibSWBF2::Types
{
	void WorldAnimationKey::WriteToStream(FileWriter& stream)
	{
		stream.WriteFloat(m_Time);
		m_Value.WriteToStream(stream);
		stream.WriteByte((uint8_t) m_TransitionType);
		m_EaseOut.WriteToStream(stream);
		m_EaseIn.WriteToStream(stream);
	}

	void WorldAnimationKey::ReadFromStream(FileReader& stream)
	{
		m_Time = stream.ReadFloat();
		m_Value.ReadFromStream(stream);
		m_TransitionType = (EWorldAnimKeyTransitionType) stream.ReadByte();
		m_EaseOut.ReadFromStream(stream);
		m_EaseIn.ReadFromStream(stream);
	}

	std::string WorldAnimationKey::ToString() const
	{
		std::string rep = fmt::format("Time: {}, Value: {}, TransitionType: {}, EaseIn: {}, EaseOut: {}", 
								m_Time, 
								m_Value.ToString(),
								WorldAnimKeyTransitionTypeToString(m_TransitionType),
								m_EaseOut.ToString(),
								m_EaseIn.ToString());
		return rep;
	}
}
