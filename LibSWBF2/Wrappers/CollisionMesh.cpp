#include "pch.h"
#include "CollisionMesh.h"
#include "InternalHelpers.h"
#include <stdint.h>


namespace LibSWBF2::Wrappers
{
    bool CollisionMesh::FromChunk(coll *collChunk, CollisionMesh& out)
    {
        if (collChunk == nullptr)
        {
            return false;
        }
        out = CollisionMesh(collChunk);
        return true;
    }
    
    CollisionMesh::CollisionMesh(coll *collChunk) : p_CollisionChunk(collChunk) {}

    CollisionMesh::CollisionMesh() : p_CollisionChunk(nullptr) {}


    std::string CollisionMesh::GetName() const
    {
        if (p_CollisionChunk == nullptr || p_CollisionChunk -> p_ChunkName == nullptr)
        {
            return "";
        }
        return p_CollisionChunk -> p_ChunkName -> m_Text;
    }


    std::string CollisionMesh::GetNodeName() const
    {
        if (p_CollisionChunk == nullptr || p_CollisionChunk -> p_NodeName == nullptr)
        {
            return "";
        }
        return p_CollisionChunk -> p_NodeName -> m_Text;
    }


    ECollisionMaskFlags CollisionMesh::GetMaskFlags() const
    {
        if (p_CollisionChunk == nullptr || p_CollisionChunk -> p_Mask == nullptr)
        {
            return ECollisionMaskFlags::All;
        }
        return p_CollisionChunk -> p_Mask -> m_MaskFlags;
    }

    std::vector<uint16_t> CollisionMesh::GetIndexBuffer() const
    {
        if (p_CollisionChunk != nullptr && m_Indicies.size() == 0) //lazy load
        {
		std::vector<TREE_LEAF *>& leaves = p_CollisionChunk -> p_Tree -> m_Leaves;
            
		for (int i = 0; i < leaves.size(); i++) {
			std::vector<uint16_t>& leaf_fan = leaves[i] -> m_Indicies;
			const auto tris = TriangleFanToTriangleList<uint16_t, uint16_t>(leaf_fan, 0);
			m_Indicies.insert(std::end(m_Indicies), std::begin(tris), std::end(tris));
		}
        }

	return m_Indicies;
    }

    std::vector<Vector3> CollisionMesh::GetVertexBuffer() const
    {
    	if (p_CollisionChunk== nullptr) {
    		return {};
    	}

	return p_CollisionChunk->p_Verts->m_Verts;
    }
        
    std::string CollisionMesh::ToString() const
    {
    	if (p_CollisionChunk == nullptr)
    	{
    		return "";
    	}

        return p_CollisionChunk -> p_ChunkName -> ToString() + "\n" +
               p_CollisionChunk -> p_Info -> ToString();
    }
}
