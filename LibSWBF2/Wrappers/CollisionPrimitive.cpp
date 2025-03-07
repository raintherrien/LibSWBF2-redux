

#include "pch.h"
#include "CollisionPrimitive.h"
#include "InternalHelpers.h"
#include <stdint.h>


namespace LibSWBF2::Wrappers
{
    std::optional<CollisionPrimitive> CollisionPrimitive::FromChunks(
        std::shared_ptr<STR<"NAME"_m>> name,
        std::shared_ptr<MASK> mask,
        std::shared_ptr<STR<"PRNT"_m>> parent,
        std::shared_ptr<XFRM> transform,
        std::shared_ptr<DATA_PRIM> fields
    ) {
        if (!transform || !name || !fields) {
            return {};
        }
        return CollisionPrimitive(name, mask, parent, transform, fields);
    }

    CollisionPrimitive::CollisionPrimitive(
        std::shared_ptr<STR<"NAME"_m>> name,
        std::shared_ptr<MASK> mask,
        std::shared_ptr<STR<"PRNT"_m>> parent,
        std::shared_ptr<XFRM> transform,
        std::shared_ptr<DATA_PRIM> fields
    ) : p_FieldsChunk(fields), p_TransformChunk(transform), p_NameChunk(name), p_MaskChunk(mask), p_ParentChunk(parent) {}


    Vector4 CollisionPrimitive::GetRotation() const
    {
        return MatrixToQuaternion(p_TransformChunk -> m_RotationMatrix);
    }

    Vector3 CollisionPrimitive::GetPosition() const
    {
        return p_TransformChunk -> m_Position;
    }

    std::string CollisionPrimitive::GetName() const
    {
        return p_NameChunk -> ToString();
    }

    std::string CollisionPrimitive::GetParentName() const
    {
        return p_ParentChunk -> ToString();
    }

    ECollisionPrimitiveType CollisionPrimitive::GetPrimitiveType() const
    {    
        return p_FieldsChunk -> m_PrimitiveType;
    }

    ECollisionMaskFlags CollisionPrimitive::GetMaskFlags() const
    {
        return p_MaskChunk == nullptr ? ECollisionMaskFlags::All : p_MaskChunk -> m_MaskFlags;
    }
    
    bool CollisionPrimitive::GetCubeDims(float_t& xOut, float_t& yOut, float_t& zOut) const
    {
        bool status = p_FieldsChunk -> m_PrimitiveType == ECollisionPrimitiveType::Cube;
        if (status)
        {
            xOut = p_FieldsChunk -> m_Field1;
            yOut = p_FieldsChunk -> m_Field2;
            zOut = p_FieldsChunk -> m_Field3;
        }
        return status;
    }

    bool CollisionPrimitive::GetCylinderDims(float_t& radiusOut, float_t& heightOut) const 
    {
        bool status = p_FieldsChunk -> m_PrimitiveType == ECollisionPrimitiveType::Cylinder;
        if (status)
        {
            radiusOut = p_FieldsChunk -> m_Field1;
            heightOut = p_FieldsChunk -> m_Field2;
        }
        return status;

    }

    bool CollisionPrimitive::GetSphereRadius(float_t &radiusOut) const 
    {
        bool status = p_FieldsChunk -> m_PrimitiveType == ECollisionPrimitiveType::Sphere;
        if (status)
        {
            radiusOut = p_FieldsChunk -> m_Field1;
        }
        return status;
    }

    std::string CollisionPrimitive::ToString() const
    {
        return p_FieldsChunk -> ToString();
    }
}


