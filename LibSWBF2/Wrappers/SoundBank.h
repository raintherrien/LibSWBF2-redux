#pragma once
#include "req.h"
#include "Types/Enums.h"
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
		SampleBank* p_soundBank;
		std::vector<Sound> m_Sounds;
		class SoundMapsWrapper* m_NameToIndexMaps;

	public:
		SoundBank() = default;
		~SoundBank() = default;

		static bool FromChunk(SampleBank* stream, SoundBank& out);

		bool HasData() const;

		const std::vector<Sound>& GetSounds() const;
		const Sound* GetSound(const std::string& soundName) const;
		const Sound* GetSound(FNVHash soundHash) const;

		ESoundFormat GetFormat() const;
		FNVHash GetHashedName() const;
	};
}
