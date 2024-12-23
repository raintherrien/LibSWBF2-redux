#pragma once
#include "req.h"
#include "Chunks/LVL/skel/skel.h"
#include "Segment.h"
#include "CollisionPrimitive.h"
#include "CollisionMesh.h"
#include <vector>

namespace LibSWBF2
{
	namespace Chunks
	{
		namespace LVL::modl
		{
			struct modl;
			struct segm;
			struct modl;
		}

		namespace skel
		{
			struct skel;
		}
	}
}


namespace LibSWBF2::Wrappers
{
	using Types::Vector3;
	using Types::Vector4;
	using Chunks::LVL::modl::modl;
	using Chunks::LVL::modl::segm;
	using Chunks::LVL::skel::skel;
	
	class Level;

	struct LIBSWBF2_API Bone
	{
		std::string m_BoneName;
		std::string m_Parent;
		Vector3 m_Position;
		Vector4 m_Rotation;
	};

	class LIBSWBF2_API Model
	{
		friend Level;
	private:
		std::shared_ptr<modl> p_Model;
		std::shared_ptr<skel> p_Skeleton;
		std::vector<Segment> m_Segments;

		CollisionMesh m_CollisionMesh;
		std::vector<CollisionPrimitive> m_CollisionPrimitives;
		std::vector<Bone> m_Bones;

	public:
		Model() = default;
		~Model() = default;

		static std::optional<Model> FromChunk(std::shared_ptr<Level> mainContainer, std::shared_ptr<modl> modelChunk);

		std::string GetName() const;
		std::vector<Segment> GetSegments() const;
		bool IsSkeletalMesh() const;
		bool IsSkeletonBroken() const;
		std::vector<Bone> GetBones() const;


		const CollisionMesh *GetCollisionMesh() const;
		std::vector<CollisionPrimitive> GetCollisionPrimitives() const;
	};
}
