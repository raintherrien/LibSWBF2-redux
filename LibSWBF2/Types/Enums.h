#pragma once
#include "../req.h"

#ifdef __cplusplus
// Required for enum to string
#include <string>
#endif

#ifdef __cplusplus
#define LIBSWBF2_ENUMCLASS enum class
#define LIBSWBF2_ENUML(E,L) L
#else
#define LIBSWBF2_ENUMCLASS enum
#define LIBSWBF2_ENUML(E,L) E##_##L
#endif

#ifdef __cplusplus
namespace LibSWBF2 {
#endif

	LIBSWBF2_ENUMCLASS ELogType : uint8_t
	{
		LIBSWBF2_ENUML(ELogType,Debug) = 0,
		LIBSWBF2_ENUML(ELogType,Info) = 1,
		LIBSWBF2_ENUML(ELogType,Warning) = 2,
		LIBSWBF2_ENUML(ELogType,Error) = 3
	};

	// Bitmap flags
	LIBSWBF2_ENUMCLASS EModelPurpose : uint16_t
	{
		// Unknown
		LIBSWBF2_ENUML(EModelPurpose,Miscellaneous) = 0,

		// Meshes
		LIBSWBF2_ENUML(EModelPurpose,Mesh) = 63,
		LIBSWBF2_ENUML(EModelPurpose,Mesh_Regular) = 1,
		LIBSWBF2_ENUML(EModelPurpose,Mesh_Lowrez) = 2,
		LIBSWBF2_ENUML(EModelPurpose,Mesh_Collision) = 4,
		LIBSWBF2_ENUML(EModelPurpose,Mesh_VehicleCollision) = 8,
		LIBSWBF2_ENUML(EModelPurpose,Mesh_ShadowVolume) = 16,
		LIBSWBF2_ENUML(EModelPurpose,Mesh_TerrainCut) = 32,

		// Just Points
		LIBSWBF2_ENUML(EModelPurpose,Point) = 448,
		LIBSWBF2_ENUML(EModelPurpose,Point_EmptyTransform) = 64,
		LIBSWBF2_ENUML(EModelPurpose,Point_DummyRoot) = 128,
		LIBSWBF2_ENUML(EModelPurpose,Point_HardPoint) = 256,

		// Skeleton
		LIBSWBF2_ENUML(EModelPurpose,Skeleton) = 7680,
		LIBSWBF2_ENUML(EModelPurpose,Skeleton_Root) = 512,
		LIBSWBF2_ENUML(EModelPurpose,Skeleton_BoneRoot) = 1024,
		LIBSWBF2_ENUML(EModelPurpose,Skeleton_BoneLimb) = 2048,
		LIBSWBF2_ENUML(EModelPurpose,Skeleton_BoneEnd) = 4096,
	};

	LIBSWBF2_ENUMCLASS EModelType : uint32_t
	{
		LIBSWBF2_ENUML(EModelType,Null) = 0,
		LIBSWBF2_ENUML(EModelType,Skin) = 1,
		LIBSWBF2_ENUML(EModelType,Envelope) = 3,
		LIBSWBF2_ENUML(EModelType,Static) = 4,
		LIBSWBF2_ENUML(EModelType,Shadow) = 6
	};

	LIBSWBF2_ENUMCLASS ELODLevel : uint8_t
	{
		LIBSWBF2_ENUML(ELODLevel,NONE) = 0,
		LIBSWBF2_ENUML(ELODLevel,LOD1) = 1,
		LIBSWBF2_ENUML(ELODLevel,LOD2) = 2,
		LIBSWBF2_ENUML(ELODLevel,LOD3) = 3,
		LIBSWBF2_ENUML(ELODLevel,LOWD) = 4
	};

	LIBSWBF2_ENUMCLASS ETopology : int32_t
	{
		LIBSWBF2_ENUML(ETopology,PointList) = 1,
		LIBSWBF2_ENUML(ETopology,LineList) = 2,
		LIBSWBF2_ENUML(ETopology,LineStrip) = 3,
		LIBSWBF2_ENUML(ETopology,TriangleList) = 4,
		LIBSWBF2_ENUML(ETopology,TriangleStrip) = 5,
		LIBSWBF2_ENUML(ETopology,TriangleFan) = 6
	};

	// copied from: https://github.com/SleepKiller/swbf-unmunge
	LIBSWBF2_ENUMCLASS EMaterialFlags : uint32_t
	{
		LIBSWBF2_ENUML(EMaterialFlags,Normal) = 1,
		LIBSWBF2_ENUML(EMaterialFlags,Hardedged) = 2,
		LIBSWBF2_ENUML(EMaterialFlags,Transparent) = 4,
		LIBSWBF2_ENUML(EMaterialFlags,Glossmap) = 8,
		LIBSWBF2_ENUML(EMaterialFlags,Glow) = 16,
		LIBSWBF2_ENUML(EMaterialFlags,BumpMap) = 32,
		LIBSWBF2_ENUML(EMaterialFlags,Additive) = 64,
		LIBSWBF2_ENUML(EMaterialFlags,Specular) = 128,
		LIBSWBF2_ENUML(EMaterialFlags,EnvMap) = 256,
		LIBSWBF2_ENUML(EMaterialFlags,VertexLighting) = 512,
		LIBSWBF2_ENUML(EMaterialFlags,TiledNormalmap) = 2048,
		LIBSWBF2_ENUML(EMaterialFlags,Doublesided) = 65536,

		LIBSWBF2_ENUML(EMaterialFlags,Scrolling) = 16777216,
		LIBSWBF2_ENUML(EMaterialFlags,Energy) = 33554432,
		LIBSWBF2_ENUML(EMaterialFlags,Animated) = 67108864,

		LIBSWBF2_ENUML(EMaterialFlags,AttachedLight) = 134217728,
	};

	LIBSWBF2_ENUMCLASS EConfigType : uint32_t
	{
		LIBSWBF2_ENUML(EConfigType,All) = 0xffffffff,
		LIBSWBF2_ENUML(EConfigType,Lighting) = 0x7468676c,
		LIBSWBF2_ENUML(EConfigType,Effect) = 0x5f5f7866,
		LIBSWBF2_ENUML(EConfigType,Boundary) = 0x5f646e62,
		LIBSWBF2_ENUML(EConfigType,Skydome) = 0x5f796b73,
		LIBSWBF2_ENUML(EConfigType,Path) = 0x68746170,
		LIBSWBF2_ENUML(EConfigType,Combo) = 0x626d6f63,
		LIBSWBF2_ENUML(EConfigType,Music) = 0x5f73756d,
		LIBSWBF2_ENUML(EConfigType,FoleyFX) = 0x5f786666,
		LIBSWBF2_ENUML(EConfigType,Sound) = 0x5f646e73,
		LIBSWBF2_ENUML(EConfigType,TriggerSoundRegion) = 0x5f727374,
		LIBSWBF2_ENUML(EConfigType,HUD) = 0x5f647568,
	};

	LIBSWBF2_ENUMCLASS ECollisionMaskFlags : uint32_t
	{
		LIBSWBF2_ENUML(ECollisionMaskFlags,All) = 0xffffffff,
		LIBSWBF2_ENUML(ECollisionMaskFlags,Ordnance) = 1,
		LIBSWBF2_ENUML(ECollisionMaskFlags,Vehicle) = 2,
		LIBSWBF2_ENUML(ECollisionMaskFlags,Building) = 4,
		LIBSWBF2_ENUML(ECollisionMaskFlags,Terrain) = 8,
		LIBSWBF2_ENUML(ECollisionMaskFlags,Soldier) = 16,
		LIBSWBF2_ENUML(ECollisionMaskFlags,Flag) = 32,
	};

	LIBSWBF2_ENUMCLASS EWorldAnimKeyTransitionType : uint8_t
	{
		LIBSWBF2_ENUML(EWorldAnimKeyTransitionType,Pop) = 0,
		LIBSWBF2_ENUML(EWorldAnimKeyTransitionType,Linear) = 1,
		LIBSWBF2_ENUML(EWorldAnimKeyTransitionType,Spline) = 2,
	};

	LIBSWBF2_ENUMCLASS ECollisionPrimitiveType : uint32_t
	{
		LIBSWBF2_ENUML(ECollisionPrimitiveType,Sphere) = 1,//can also be 0...
		LIBSWBF2_ENUML(ECollisionPrimitiveType,Cylinder) = 2,
		LIBSWBF2_ENUML(ECollisionPrimitiveType,Empty) = 3, //not confirmed in documentation read so far
		LIBSWBF2_ENUML(ECollisionPrimitiveType,Cube) = 4,
	};

	LIBSWBF2_ENUMCLASS EVBUFFlags : uint32_t
	{
		LIBSWBF2_ENUML(EVBUFFlags,None) = 0b0u,
		LIBSWBF2_ENUML(EVBUFFlags,Position) = 0b10u,
		LIBSWBF2_ENUML(EVBUFFlags,Unknown1) = 0b100u,		// definetly not bone indices

		// this is questionable... not found in all of sides/rep.lvl. Another UV maybe (2 float)?
		LIBSWBF2_ENUML(EVBUFFlags,BlendWeight) = 0b1000u,

		LIBSWBF2_ENUML(EVBUFFlags,Normal) = 0b100000u,
		LIBSWBF2_ENUML(EVBUFFlags,Tangents) = 0b1000000u,
		LIBSWBF2_ENUML(EVBUFFlags,Color) = 0b10000000u,
		LIBSWBF2_ENUML(EVBUFFlags,StaticLighting) = 0b100000000u,
		LIBSWBF2_ENUML(EVBUFFlags,TexCoord) = 0b1000000000u,

		LIBSWBF2_ENUML(EVBUFFlags,PositionCompressed) = 0b1000000000000u,
		LIBSWBF2_ENUML(EVBUFFlags,BlendWeightCompressed) = 0b10000000000000u,
		LIBSWBF2_ENUML(EVBUFFlags,NormalCompressed) = 0b100000000000000u,
		LIBSWBF2_ENUML(EVBUFFlags,TexCoordCompressed) = 0b1000000000000000u
	};

	LIBSWBF2_ENUMCLASS ETextureFormat : uint8_t
	{
		LIBSWBF2_ENUML(ETextureFormat,R8_G8_B8_A8),
		LIBSWBF2_ENUML(ETextureFormat,B8_G8_R8_A8)
	};

	LIBSWBF2_ENUMCLASS ETerrainBufferType : uint32_t
	{
		LIBSWBF2_ENUML(ETerrainBufferType,Geometry) = 290,
		LIBSWBF2_ENUML(ETerrainBufferType,Texture) = 20770,
		LIBSWBF2_ENUML(ETerrainBufferType,TextureExtra) = 130
	};

	LIBSWBF2_ENUMCLASS ELightType : uint32_t
	{
		LIBSWBF2_ENUML(ELightType,Omni) = 2,
		LIBSWBF2_ENUML(ELightType,Dir) = 1,
		LIBSWBF2_ENUML(ELightType,Spot) = 3,
		LIBSWBF2_ENUML(ELightType,Unknown) = 0,
	};

	LIBSWBF2_ENUMCLASS EEntityClassType
	{
		LIBSWBF2_ENUML(EEntityClassType,GameObjectClass),
		LIBSWBF2_ENUML(EEntityClassType,OrdnanceClass),
		LIBSWBF2_ENUML(EEntityClassType,WeaponClass),
		LIBSWBF2_ENUML(EEntityClassType,ExplosionClass)
	};

	LIBSWBF2_ENUMCLASS ESoundFormat : uint32_t
	{
		LIBSWBF2_ENUML(ESoundFormat,PCM8) = 1,
		LIBSWBF2_ENUML(ESoundFormat,PCM16) = 2,
		LIBSWBF2_ENUML(ESoundFormat,VAG) = 3,
		LIBSWBF2_ENUML(ESoundFormat,XADPCM) = 4,
		LIBSWBF2_ENUML(ESoundFormat,IMAADPCM) = 5,
		LIBSWBF2_ENUML(ESoundFormat,Unity) = 6
	};


	LIBSWBF2_ENUMCLASS EArcFilterFlags : uint32_t
	{
		LIBSWBF2_ENUML(EArcFilterFlags,Soldier) = 1,
		LIBSWBF2_ENUML(EArcFilterFlags,Small) = 2,
		LIBSWBF2_ENUML(EArcFilterFlags,Medium) = 4,
		LIBSWBF2_ENUML(EArcFilterFlags,Hover) = 8,
		LIBSWBF2_ENUML(EArcFilterFlags,Large) = 16,
		LIBSWBF2_ENUML(EArcFilterFlags,Huge) = 32
	};


	LIBSWBF2_ENUMCLASS EArcAttributeFlags : uint32_t
	{
		LIBSWBF2_ENUML(EArcAttributeFlags,OneWay) = 1,
		LIBSWBF2_ENUML(EArcAttributeFlags,Jump) = 2,
		LIBSWBF2_ENUML(EArcAttributeFlags,JetJump) = 4
	};

#ifdef __cplusplus
	std::string LIBSWBF2_API ConfigTypeToString(EConfigType type);
	std::string LIBSWBF2_API LogTypeToString(ELogType type);
	std::string LIBSWBF2_API TopologyToString(ETopology topology);
	std::string LIBSWBF2_API MaterialFlagsToString(EMaterialFlags flags);
	std::string LIBSWBF2_API ModelPurposeToString(EModelPurpose flags);
	std::string LIBSWBF2_API VBUFFlagsToString(EVBUFFlags flags);
	std::string LIBSWBF2_API TerrainBufferTypeToString(ETerrainBufferType type);
	std::string LIBSWBF2_API LightTypeToString(ELightType type);
	std::string LIBSWBF2_API EntityClassToString(EEntityClassType type);
	std::string LIBSWBF2_API CollisionMaskTypeToString(ECollisionMaskFlags type);
	std::string LIBSWBF2_API CollisionPrimitiveTypeToString(ECollisionPrimitiveType type);
	std::string LIBSWBF2_API WorldAnimKeyTransitionTypeToString(EWorldAnimKeyTransitionType type);
	std::string LIBSWBF2_API SoundFormatToString(ESoundFormat format);
	std::string LIBSWBF2_API ArcFilterToString(EArcFilterFlags filter);
	std::string LIBSWBF2_API ArcAttributesToString(EArcAttributeFlags attributes);

	EMaterialFlags LIBSWBF2_API operator &(EMaterialFlags lhs, EMaterialFlags rhs);
	bool LIBSWBF2_API operator ==(EMaterialFlags lhs, std::underlying_type<EMaterialFlags>::type rhs);
	bool LIBSWBF2_API operator !=(EMaterialFlags lhs, std::underlying_type<EMaterialFlags>::type rhs);

	EModelPurpose LIBSWBF2_API operator &(EModelPurpose lhs, EModelPurpose rhs);
	EModelPurpose LIBSWBF2_API operator |(EModelPurpose lhs, EModelPurpose rhs);
	bool LIBSWBF2_API operator ==(EModelPurpose lhs, std::underlying_type<EModelPurpose>::type rhs);
	bool LIBSWBF2_API operator !=(EModelPurpose lhs, std::underlying_type<EModelPurpose>::type rhs);

	EVBUFFlags LIBSWBF2_API operator &(EVBUFFlags lhs, EVBUFFlags rhs);
	bool LIBSWBF2_API operator ==(EVBUFFlags lhs, std::underlying_type<EVBUFFlags>::type rhs);
	bool LIBSWBF2_API operator !=(EVBUFFlags lhs, std::underlying_type<EVBUFFlags>::type rhs);

	ECollisionMaskFlags LIBSWBF2_API operator &(ECollisionMaskFlags lhs, ECollisionMaskFlags rhs);
	bool LIBSWBF2_API operator ==(ECollisionMaskFlags lhs, std::underlying_type<ECollisionMaskFlags>::type rhs);
	bool LIBSWBF2_API operator !=(ECollisionMaskFlags lhs, std::underlying_type<ECollisionMaskFlags>::type rhs);

	bool LIBSWBF2_API operator ==(EConfigType lhs, std::underlying_type<EConfigType>::type rhs);

	bool LIBSWBF2_API operator ==(EWorldAnimKeyTransitionType lhs, std::underlying_type<EWorldAnimKeyTransitionType>::type rhs);

	bool LIBSWBF2_API operator ==(ESoundFormat lhs, std::underlying_type<ESoundFormat>::type rhs);

	EArcFilterFlags LIBSWBF2_API operator &(EArcFilterFlags lhs, EArcFilterFlags rhs);
	bool LIBSWBF2_API operator ==(EArcFilterFlags lhs, std::underlying_type<EArcFilterFlags>::type rhs);
	bool LIBSWBF2_API operator !=(EArcFilterFlags lhs, std::underlying_type<EArcFilterFlags>::type rhs);

	EArcAttributeFlags LIBSWBF2_API operator &(EArcAttributeFlags lhs, EArcAttributeFlags rhs);
	bool LIBSWBF2_API operator ==(EArcAttributeFlags lhs, std::underlying_type<EArcAttributeFlags>::type rhs);
	bool LIBSWBF2_API operator !=(EArcAttributeFlags lhs, std::underlying_type<EArcAttributeFlags>::type rhs);
#endif

#ifdef __cplusplus
} // namespace LibSWBF2
#endif
