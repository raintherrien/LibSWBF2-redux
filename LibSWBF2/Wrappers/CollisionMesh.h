#pragma once
#include "Chunks/LVL/coll/coll.h"
#include "Chunks/STR.h"
#include "Types/Vector3.h"
#include <string>
#include <vector>

namespace LibSWBF2::Wrappers
{
	using namespace LibSWBF2::Chunks::LVL::coll;
	using namespace LibSWBF2::Types;
	using namespace LibSWBF2::Chunks;

	class LIBSWBF2_API CollisionMesh
	{
		friend class Level;
		friend class Model;
		
	private:

		static std::optional<CollisionMesh> FromChunk(std::shared_ptr<coll> fieldsChunk);
		
		CollisionMesh(std::shared_ptr<coll> fieldsChunk);
		CollisionMesh(); 

		std::shared_ptr<coll> p_CollisionChunk;
		mutable std::vector<uint16_t> m_Indicies;


	public:
		
		std::string GetName() const;

		std::vector<uint16_t> GetIndexBuffer() const;
		std::vector<Vector3> GetVertexBuffer() const;

		ECollisionMaskFlags GetMaskFlags() const;

		std::string ToString() const;

		std::string GetNodeName() const;
	};
}
