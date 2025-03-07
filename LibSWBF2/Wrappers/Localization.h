#pragma once
#include <string>

namespace LibSWBF2::Chunks::LVL::Localization
{
	struct Locl;
}

namespace LibSWBF2::Wrappers
{
	using LibSWBF2::Chunks::LVL::Localization::Locl;

	class LIBSWBF2_API Localization
	{
	private:
		friend class Level;

		Localization& operator=(const Localization& other);
		Localization& operator=(Localization&& other);

	private:
		std::shared_ptr<Locl> p_Locl;
		class LocalizationMaps* m_LocalizationMaps;

	public:
		Localization();
		~Localization();
		Localization(const Localization &);

		static std::optional<Localization> FromChunk(std::shared_ptr<Locl> loclChunk);

		const std::string& GetName() const;

		// count is number of wide chars, NOT number of bytes!
		bool GetLocalizedWideString(const std::string& path, const wchar_t*& chars, uint32_t& count) const;
	};
}
