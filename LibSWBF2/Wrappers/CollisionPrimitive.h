#pragma once
#include "Chunks/LVL/prim/prim.h"
#include "Chunks/LVL/prim/prim.DATA.h"
#include "Chunks/LVL/prim/MASK.h"
#include "Chunks/LVL/wrld/XFRM.h"
#include "Chunks/STR.h"

#include "Types/Vector4.h"
#include "Types/Vector3.h"
#include "Types/Matrix3x3.h"

#include <string>

namespace LibSWBF2::Wrappers
{
	using namespace LibSWBF2::Chunks::LVL::prim;
	using namespace LibSWBF2::Chunks::LVL::wrld;
	using namespace LibSWBF2::Types;
	using namespace LibSWBF2::Chunks;

	class LIBSWBF2_API CollisionPrimitive
	{
		friend class Level;

	private:
		
		std::shared_ptr<DATA_PRIM> p_FieldsChunk;
		std::shared_ptr<XFRM> p_TransformChunk;
		std::shared_ptr<STR<"NAME"_m>> p_NameChunk;
		std::shared_ptr<MASK> p_MaskChunk;
		std::shared_ptr<STR<"PRNT"_m>> p_ParentChunk;

		
		static std::optional<CollisionPrimitive> FromChunks(
			std::shared_ptr<STR<"NAME"_m>> name,
			std::shared_ptr<MASK> mask,
			std::shared_ptr<STR<"PRNT"_m>> parent,
			std::shared_ptr<XFRM> transform,
			std::shared_ptr<DATA_PRIM> fields
		);
	public:
		CollisionPrimitive(
			std::shared_ptr<STR<"NAME"_m>> name,
			std::shared_ptr<MASK> mask,
			std::shared_ptr<STR<"PRNT"_m>> parent,
			std::shared_ptr<XFRM> transform,
			std::shared_ptr<DATA_PRIM> fields
		);
		
		std::string GetName() const;
		std::string GetParentName() const;

		Vector4 GetRotation() const;
		Vector3 GetPosition() const;

		ECollisionPrimitiveType GetPrimitiveType() const;
		ECollisionMaskFlags GetMaskFlags() const;

		bool GetCubeDims(float_t& xOut, float_t& yOut, float_t& zOut) const;
		bool GetCylinderDims(float_t& radiusOut, float_t& heightOut) const;
		bool GetSphereRadius(float_t &radiusOut) const;
		
		std::string ToString() const;
	};
}
