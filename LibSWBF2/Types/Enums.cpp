#include "pch.h"
#include "Enums.h"
#include <string>
#include <fmt/format.h>

namespace LibSWBF2
{
	std::string ConfigTypeToString(EConfigType type)
	{
		switch (type) {
			case EConfigType::All:
				return "All";
			case EConfigType::Lighting:
				return "Lighting";
			case EConfigType::Effect:
				return "Effect";
			case EConfigType::Boundary:
				return "Boundary";
			case EConfigType::Skydome:
				return "Skydome";
			case EConfigType::Path:
				return "Path";
			case EConfigType::Combo:
				return "Combo";
			case EConfigType::Music:
				return "Music";
			case EConfigType::FoleyFX:
				return "FoleyFX";
			case EConfigType::Sound:
				return "Sound";
			case EConfigType::TriggerSoundRegion:
				return "TriggerSoundRegion";
			case EConfigType::HUD:
				return "HUD";
		}
		return fmt::format("Unknown ConfigLog Type: {}", (int)type);
	}

	std::string LogTypeToString(ELogType type)
	{
		switch (type)
		{
			case ELogType::Debug:
				return "Debug";
			case ELogType::Info:
				return "Info";
			case ELogType::Warning:
				return "Warning";
			case ELogType::Error:
				return "Error";
		}
		return fmt::format("Unknown Log Type: {}", (int)type);
	}

	std::string TopologyToString(ETopology topology)
	{
		switch (topology)
		{
			case ETopology::PointList:
				return "Point List";
			case ETopology::LineList:
				return "Line List";
			case ETopology::LineStrip:
				return "Line Strip";
			case ETopology::TriangleList:
				return "Triangle List";
			case ETopology::TriangleStrip:
				return "Triangle Strip";
			case ETopology::TriangleFan:
				return "Triangle Strip";
		}
		return fmt::format("Unknown Topology: {}", (int)topology);
	}

	std::string MaterialFlagsToString(EMaterialFlags flags)
	{
		std::string result = "[";
		if ((flags & EMaterialFlags::Normal) != 0)
		{
			result += "Normal, ";
		}
		if ((flags & EMaterialFlags::Hardedged) != 0)
		{
			result += "Hardedged, ";
		}
		if ((flags & EMaterialFlags::Transparent) != 0)
		{
			result += "Transparent, ";
		}
		if ((flags & EMaterialFlags::Glossmap) != 0)
		{
			result += "Glossmap, ";
		}
		if ((flags & EMaterialFlags::Glow) != 0)
		{
			result += "Glow, ";
		}
		if ((flags & EMaterialFlags::BumpMap) != 0)
		{
			result += "BumpMap, ";
		}
		if ((flags & EMaterialFlags::Additive) != 0)
		{
			result += "Additive, ";
		}
		if ((flags & EMaterialFlags::Specular) != 0)
		{
			result += "Specular, ";
		}
		if ((flags & EMaterialFlags::EnvMap) != 0)
		{
			result += "EnvMap, ";
		}
		if ((flags & EMaterialFlags::VertexLighting) != 0)
		{
			result += "VertexLighting, ";
		}
		if ((flags & EMaterialFlags::TiledNormalmap) != 0)
		{
			result += "TiledNormalmap, ";
		}
		if ((flags & EMaterialFlags::Doublesided) != 0)
		{
			result += "Doublesided, ";
		}
		if ((flags & EMaterialFlags::Scrolling) != 0)
		{
			result += "Scrolling, ";
		}
		if ((flags & EMaterialFlags::Energy) != 0)
		{
			result += "Energy, ";
		}
		if ((flags & EMaterialFlags::Animated) != 0)
		{
			result += "Animated, ";
		}
		if ((flags & EMaterialFlags::AttachedLight) != 0)
		{
			result += "AttachedLight, ";
		}

		size_t resultSize = result.size();
		if (resultSize > 1)
		{
			result.resize(resultSize - 2); //failed w/overflow when length == 1 eg "["
		}

		result += "]";
		return result;
	}

	std::string ModelPurposeToString(EModelPurpose purpose)
	{
		switch (purpose)
		{
			case EModelPurpose::Miscellaneous:
				return "Miscellaneous";

			case EModelPurpose::Mesh:
				return "Mesh";
			case EModelPurpose::Mesh_Regular:
				return "Mesh_Regular";
			case EModelPurpose::Mesh_Lowrez:
				return "Mesh_Lowrez";
			case EModelPurpose::Mesh_Collision:
				return "Mesh_Collision";
			case EModelPurpose::Mesh_VehicleCollision:
				return "Mesh_VehicleCollision";
			case EModelPurpose::Mesh_ShadowVolume:
				return "Mesh_ShadowVolume";
			case EModelPurpose::Mesh_TerrainCut:
				return "Mesh_TerrainCut";

			case EModelPurpose::Point:
				return "Point";
			case EModelPurpose::Point_EmptyTransform:
				return "Point_EmptyTransform";
			case EModelPurpose::Point_DummyRoot:
				return "Point_DummyRoot";
			case EModelPurpose::Point_HardPoint:
				return "Point_HardPoint";

			case EModelPurpose::Skeleton:
				return "Skeleton";
			case EModelPurpose::Skeleton_Root:
				return "Skeleton_Root";
			case EModelPurpose::Skeleton_BoneRoot:
				return "Skeleton_BoneRoot";
			case EModelPurpose::Skeleton_BoneLimb:
				return "Skeleton_BoneLimb";
			case EModelPurpose::Skeleton_BoneEnd:
				return "Skeleton_BoneEnd";
		}
		return fmt::format("Unknown Model Purpose: {}", (int)purpose);
	}

	std::string VBUFFlagsToString(EVBUFFlags flags)
	{
		std::string result = "[";
		if ((flags & EVBUFFlags::Position) != 0)
		{
			result += "Position, ";
		}
		if ((flags & EVBUFFlags::Unknown1) != 0)
		{
			result += "Unknown 1, ";
		}
		if ((flags & EVBUFFlags::BlendWeight) != 0)
		{
			result += "BlendWeight, ";
		}
		if ((flags & EVBUFFlags::Normal) != 0)
		{
			result += "Normal, ";
		}
		if ((flags & EVBUFFlags::Tangents) != 0)
		{
			result += "Tangents, ";
		}
		if ((flags & EVBUFFlags::Color) != 0)
		{
			result += "Color, ";
		}
		if ((flags & EVBUFFlags::StaticLighting) != 0)
		{
			result += "StaticLighting, ";
		}
		if ((flags & EVBUFFlags::TexCoord) != 0)
		{
			result += "TexCoord, ";
		}
		if ((flags & EVBUFFlags::PositionCompressed) != 0)
		{
			result += "PositionCompressed, ";
		}
		if ((flags & EVBUFFlags::BlendWeightCompressed) != 0)
		{
			result += "BlendInfoCompressed, ";
		}
		if ((flags & EVBUFFlags::NormalCompressed) != 0)
		{
			result += "NormalCompressed, ";
		}
		if ((flags & EVBUFFlags::TexCoordCompressed) != 0)
		{
			result += "TexCoordCompressed, ";
		}
		result.resize(result.size() - 2);
		result += "]";
		return result;
	}

	std::string TerrainBufferTypeToString(ETerrainBufferType type)
	{
		switch (type)
		{
			case ETerrainBufferType::Geometry:
				return "Geometry";
			case ETerrainBufferType::Texture:
				return "Texture";
			case ETerrainBufferType::TextureExtra:
				return "TextureExtra";
			default:
				return fmt::format("Unknown ETerrainBufferType: {}", (int)type);
		}
	}

	std::string LIBSWBF2_API LightTypeToString(ELightType type)
	{
		switch (type)
		{
			case ELightType::Omni:
				return "Omnidirectional";
			case ELightType::Spot:
				return "Spot";
			case ELightType::Dir:
				return "Directional";
			default:
				return "Unknown";
		}
	}

	std::string EntityClassToString(EEntityClassType type)
	{
		switch (type)
		{
			case EEntityClassType::GameObjectClass:
				return "GameObjectClass";
			case EEntityClassType::OrdnanceClass:
				return "OrdnanceClass";
			case EEntityClassType::WeaponClass:
				return "WeaponClass";
			case EEntityClassType::ExplosionClass:
				return "ExplosionClass";
			default:
				return fmt::format("Unknown EEntityClassType: {}", (int)type);
		}
	}

	std::string CollisionMaskTypeToString(ECollisionMaskFlags type)
	{
		std::string result = "[";

        if ((type & ECollisionMaskFlags::Ordnance) != 0)
        {
            result += "Ordnance, ";
        }

        if ((type & ECollisionMaskFlags::Vehicle) != 0)
        {
            result += "Vehicle, ";
        }

        if ((type & ECollisionMaskFlags::Building) != 0)
        {
            result += "Building, ";
        }

        if ((type & ECollisionMaskFlags::Terrain) != 0)
        {
            result += "Terrain, ";
        }

        if ((type & ECollisionMaskFlags::Soldier) != 0)
        {
            result += "Soldier, ";
        }

        if ((type & ECollisionMaskFlags::Flag) != 0)
        {
            result += "Flag, ";
        }

        size_t resultSize = result.size();

        if (resultSize > 1)
        {
        	result.resize(result.size() - 2);
        	result += "]";
        } else 
        {
        	result.resize(0);
        	result += "[All]";
        }

		return result;
	}

	std::string CollisionPrimitiveTypeToString(ECollisionPrimitiveType type)
	{
		if (type == ECollisionPrimitiveType::Sphere || (uint32_t) type == 0x0)
		{
			return "Sphere";
		}

		if (type == ECollisionPrimitiveType::Cylinder)
		{
			return "Cylinder";
		}

		if (type == ECollisionPrimitiveType::Cube)
		{
			return "Cube";
		}

		if (type == ECollisionPrimitiveType::Empty)
		{
			return "Empty";
		}

		return fmt::format("Unknown Collision Primitive Type: {}", (uint32_t)type);
	}

	std::string WorldAnimKeyTransitionTypeToString(EWorldAnimKeyTransitionType type)
	{
		switch (type)
		{
			case EWorldAnimKeyTransitionType::Pop:
				return "Pop";
			case EWorldAnimKeyTransitionType::Linear:
				return "Linear";		
			case EWorldAnimKeyTransitionType::Spline:
				return "Spline";
			default:
				return fmt::format("Unknown EWorldAnimKeyTransitionType: {}", (uint8_t)type);
		}
	}

	std::string LIBSWBF2_API SoundFormatToString(ESoundFormat format)
	{
		switch (format)
		{
			case ESoundFormat::PCM8:
				return "PCM8";
			case ESoundFormat::PCM16:
				return "PCM16";
			case ESoundFormat::VAG:
				return "VAG";
			case ESoundFormat::XADPCM:
				return "X ADPCM";
			case ESoundFormat::IMAADPCM:
				return "IMA ADPCM";
			case ESoundFormat::Unity:
				return "Unity";
			default:
				return fmt::format("Unknown ESoundFormat: {}", (uint32_t)format);
		}	
	}

	std::string LIBSWBF2_API ArcFilterToString(EArcFilterFlags flags)
	{
		std::string result = "[";
		if ((flags & EArcFilterFlags::Soldier) != 0)
		{
			result += "Soldier, ";
		}
		if ((flags & EArcFilterFlags::Small) != 0)
		{
			result += "Small, ";
		}
		if ((flags & EArcFilterFlags::Medium) != 0)
		{
			result += "Medium, ";
		}
		if ((flags & EArcFilterFlags::Hover) != 0)
		{
			result += "Hover, ";
		}
		if ((flags & EArcFilterFlags::Large) != 0)
		{
			result += "Large, ";
		}
		if ((flags & EArcFilterFlags::Huge) != 0)
		{
			result += "Huge, ";
		}

		size_t resultSize = result.size();
		if (resultSize > 1)
		{
			result.resize(resultSize - 2);
		}

		result += "]";
		return result;
	}

	std::string LIBSWBF2_API ArcAttributesToString(EArcAttributeFlags flags)
	{
		std::string result = "[";
		if ((flags & EArcAttributeFlags::OneWay) != 0)
		{
			result += "OneWay, ";
		}
		if ((flags & EArcAttributeFlags::Jump) != 0)
		{
			result += "Jump, ";
		}
		if ((flags & EArcAttributeFlags::JetJump) != 0)
		{
			result += "JetJump, ";
		}

		size_t resultSize = result.size();
		if (resultSize > 1)
		{
			result.resize(resultSize - 2);
		}

		result += "]";
		return result;
	}

	
	EMaterialFlags operator &(EMaterialFlags lhs, EMaterialFlags rhs)
	{
		return static_cast<EMaterialFlags> (
			static_cast<std::underlying_type<EMaterialFlags>::type>(lhs) &
			static_cast<std::underlying_type<EMaterialFlags>::type>(rhs)
			);
	}

	bool operator ==(EMaterialFlags lhs, std::underlying_type<EMaterialFlags>::type rhs)
	{
		return static_cast<std::underlying_type<EMaterialFlags>::type>(lhs) == rhs;
	}

	bool operator !=(EMaterialFlags lhs, std::underlying_type<EMaterialFlags>::type rhs)
	{
		return static_cast<std::underlying_type<EMaterialFlags>::type>(lhs) != rhs;
	}

	EModelPurpose operator &(EModelPurpose lhs, EModelPurpose rhs)
	{
		return static_cast<EModelPurpose> (
			static_cast<std::underlying_type<EModelPurpose>::type>(lhs) &
			static_cast<std::underlying_type<EModelPurpose>::type>(rhs)
			);
	}

	EModelPurpose operator |(EModelPurpose lhs, EModelPurpose rhs)
	{
		return static_cast<EModelPurpose> (
			static_cast<std::underlying_type<EModelPurpose>::type>(lhs) |
			static_cast<std::underlying_type<EModelPurpose>::type>(rhs)
			);
	}

	bool operator ==(EModelPurpose lhs, std::underlying_type<EModelPurpose>::type rhs)
	{
		return static_cast<std::underlying_type<EModelPurpose>::type>(lhs) == rhs;
	}

	bool operator !=(EModelPurpose lhs, std::underlying_type<EModelPurpose>::type rhs)
	{
		return static_cast<std::underlying_type<EModelPurpose>::type>(lhs) != rhs;
	}

	EVBUFFlags operator &(EVBUFFlags lhs, EVBUFFlags rhs)
	{
		return static_cast<EVBUFFlags> (
			static_cast<std::underlying_type<EVBUFFlags>::type>(lhs) &
			static_cast<std::underlying_type<EVBUFFlags>::type>(rhs)
			);
	}

	bool operator ==(EVBUFFlags lhs, std::underlying_type<EVBUFFlags>::type rhs)
	{
		return static_cast<std::underlying_type<EVBUFFlags>::type>(lhs) == rhs;
	}

	bool operator !=(EVBUFFlags lhs, std::underlying_type<EVBUFFlags>::type rhs)
	{
		return static_cast<std::underlying_type<EVBUFFlags>::type>(lhs) != rhs;
	}


	ECollisionMaskFlags operator &(ECollisionMaskFlags lhs, ECollisionMaskFlags rhs)
	{
		return static_cast<ECollisionMaskFlags> (
			static_cast<std::underlying_type<ECollisionMaskFlags>::type>(lhs) &
			static_cast<std::underlying_type<ECollisionMaskFlags>::type>(rhs)
			);
	}

	bool operator ==(ECollisionMaskFlags lhs, std::underlying_type<ECollisionMaskFlags>::type rhs)
	{
		return static_cast<std::underlying_type<ECollisionMaskFlags>::type>(lhs) == rhs;
	}

	bool operator !=(ECollisionMaskFlags lhs, std::underlying_type<ECollisionMaskFlags>::type rhs)
	{
		return static_cast<std::underlying_type<ECollisionMaskFlags>::type>(lhs) != rhs;
	}

	bool operator ==(EConfigType lhs, std::underlying_type<EConfigType>::type rhs)
	{
		return static_cast<std::underlying_type<EConfigType>::type>(lhs) == rhs;
	}

	bool operator ==(EWorldAnimKeyTransitionType lhs, std::underlying_type<EWorldAnimKeyTransitionType>::type rhs)
	{
		return static_cast<std::underlying_type<EWorldAnimKeyTransitionType>::type>(lhs) == rhs;
	}

	bool operator ==(ESoundFormat lhs, std::underlying_type<ESoundFormat>::type rhs)
	{
		return static_cast<std::underlying_type<ESoundFormat>::type>(lhs) == rhs;		
	}

	EArcFilterFlags operator &(EArcFilterFlags lhs, EArcFilterFlags rhs)
	{
		return static_cast<EArcFilterFlags> (
			static_cast<std::underlying_type<EArcFilterFlags>::type>(lhs) &
			static_cast<std::underlying_type<EArcFilterFlags>::type>(rhs)
			);
	}

	bool operator ==(EArcFilterFlags lhs, std::underlying_type<EArcFilterFlags>::type rhs)
	{
		return static_cast<std::underlying_type<EArcFilterFlags>::type>(lhs) == rhs;
	}

	bool operator !=(EArcFilterFlags lhs, std::underlying_type<EArcFilterFlags>::type rhs)
	{
		return static_cast<std::underlying_type<EArcFilterFlags>::type>(lhs) != rhs;
	}


	
	EArcAttributeFlags operator &(EArcAttributeFlags lhs, EArcAttributeFlags rhs)
	{
		return static_cast<EArcAttributeFlags> (
			static_cast<std::underlying_type<EArcAttributeFlags>::type>(lhs) &
			static_cast<std::underlying_type<EArcAttributeFlags>::type>(rhs)
			);
	}

	bool operator ==(EArcAttributeFlags lhs, std::underlying_type<EArcAttributeFlags>::type rhs)
	{
		return static_cast<std::underlying_type<EArcAttributeFlags>::type>(lhs) == rhs;
	}

	bool operator !=(EArcAttributeFlags lhs, std::underlying_type<EArcAttributeFlags>::type rhs)
	{
		return static_cast<std::underlying_type<EArcAttributeFlags>::type>(lhs) != rhs;
	}
}
