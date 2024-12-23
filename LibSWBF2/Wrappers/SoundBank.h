#pragma once
#include "req.h"
#include "Types/Enums.h"
#include "InternalHelpers.h"
#include "Sound.h"
#include <string>
#include <vector>

namespace LibSWBF2
{
	class Container;

	namespace Chunks::LVL::sound
	{
		struct SampleBank;
	}	
}

namespace LibSWBF2::Wrappers
{
	using LibSWBF2::Chunks::LVL::sound::SampleBank;

	class LIBSWBF2_API SoundBank
	{
	private:
		friend Level;

	private:
		std::shared_ptr<SampleBank> p_soundBank;
		std::vector<Sound> m_Sounds;
		SoundMapsWrapper m_NameToIndexMaps;

	public:
		static std::optional<SoundBank> FromChunk(std::shared_ptr<SampleBank> stream);

		bool HasData() const;

		const std::vector<Sound>& GetSounds() const;
		const Sound* GetSound(const std::string& soundName) const;
		const Sound* GetSound(FNVHash soundHash) const;

		ESoundFormat GetFormat() const;
		FNVHash GetHashedName() const;
	};
}
