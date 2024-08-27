#pragma once

#include "RenderingEngine/Model.h"
#include "PhysicsEngine/Animation/Bone.h"
#include "PhysicsEngine/Animation/Animation.h"
#include "PhysicsEngine/Animation/Animator.h"

namespace Nifty
{
	/*
	class AnimatedModel : public SaveItem
	{
	private:
		std::vector<Mesh> meshes;
		std::string directory;
		std::string model_path;
		std::vector<Texture> textures_loaded;
		std::vector<unsigned int> textureTypeAmounts;

		Bone* m_RootBone;
		unsigned int m_BoneCount;
		Animator* m_Animator;

	public:
		Model* model;
		unsigned int model_id;

		AnimatedModel(Model* _model, Bone* rootBone, unsigned int boneCount = 0, unsigned int id = 0)
			: model(_model), m_RootBone(rootBone), m_BoneCount(boneCount), model_id(id)
		{
			m_Animator = new Animator();
			rootBone->CalcInverseBindTransform(glm::mat4(1.0f));
		}

		Bone* GetRootBone() const { return m_RootBone; }

		void PerformAnimation(Animation* animation)
		{
			m_Animator->PerformAnimation(animation);
		}

		void Update(float deltaTime)
		{
			m_Animator->Update(deltaTime, this);
		}

		std::vector<glm::mat4> GetBoneTransforms()
		{
			std::vector<glm::mat4> boneMatrices;
			AddBonesToArray(m_RootBone, boneMatrices);
			return boneMatrices;
		}

		void AddBonesToArray(Bone* rootBone, std::vector<glm::mat4>& boneMatrices)
		{
			boneMatrices[rootBone->GetIndex()] = rootBone->GetAnimatedTransform();
			for (Bone* child : rootBone->GetChildren())
			{
				AddBonesToArray(child, boneMatrices);
			}
		}
	};
	*/
}