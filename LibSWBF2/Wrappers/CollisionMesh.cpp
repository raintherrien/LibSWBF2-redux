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

    void CollisionMesh::GetIndexBuffer(ETopology requestedTopology, uint32_t& count, uint16_t*& indexBuffer) const
    {
        if (requestedTopology != ETopology::TriangleList || p_CollisionChunk == nullptr)
        {
            count = 0;
            indexBuffer = nullptr;
            return;
        }

        if (m_Indicies.size() == 0) //lazy load
        {
		std::vector<TREE_LEAF *>& leaves = p_CollisionChunk -> p_Tree -> m_Leaves;
            
            for (int i = 0; i < leaves.size(); i++)
            {
		    std::vector<uint16_t>& leaf_fan = leaves[i] -> m_Indicies;
			const auto tris = TriangleFanToTriangleList<uint16_t, uint16_t>(leaf_fan, 0);
			m_Indicies.insert(std::end(m_Indicies), std::begin(tris), std::end(tris));
            }
        }

        count = (uint32_t)m_Indicies.size();
        indexBuffer = m_Indicies.data();
    }

    void CollisionMesh::GetVertexBuffer(uint32_t& count, Vector3*& vertexBuffer) const
    {
    	if (p_CollisionChunk== nullptr)
    	{
    		count = 0;
            vertexBuffer = nullptr;
    		return;
    	}

	std::vector<Vector3>& verts = p_CollisionChunk -> p_Verts -> m_Verts;
        count = (uint32_t) verts.size();
        vertexBuffer = verts.data();
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
