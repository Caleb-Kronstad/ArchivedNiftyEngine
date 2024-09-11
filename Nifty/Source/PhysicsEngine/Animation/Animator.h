#pragma once

namespace Nifty
{
	
	//class Animator
	//{
	//private:
	//	Animation* m_CurrentAnimation;
	//	float m_AnimationTime = 0.0f;

	//public:

	//	Animator()
	//	{

	//	}

	//	void PerformAnimation(Animation* animation)
	//	{
	//		m_AnimationTime = 0.0f;
	//		m_CurrentAnimation = animation;
	//	}

	//	void Update(float deltaTime, SkinnedMesh* parentMesh)
	//	{
	//		if (m_CurrentAnimation == nullptr) return;
	//		IncreaseAnimationTime(deltaTime);
	//		std::map<std::string, glm::mat4> currentPose = CalcCurrentAnimationPose();
	//		ApplyPoseToBones(currentPose, parentMesh->GetRootBone(), glm::mat4(1.0f));
	//	}

	//	void IncreaseAnimationTime(float deltaTime)
	//	{
	//		m_AnimationTime += deltaTime;
	//		if (m_AnimationTime > m_CurrentAnimation->GetLength())
	//			m_AnimationTime = fmod(m_AnimationTime, m_CurrentAnimation->GetLength());
	//	}

	//	std::map<std::string, glm::mat4> CalcCurrentAnimationPose()
	//	{
	//		std::vector<KeyFrame*> frames = GetPreviousAndNextFrames();
	//		float progression = CalculateProgression(frames[0], frames[1]);
	//		return InterpolatePoses(frames[0], frames[1], progression);
	//	}

	//	void ApplyPoseToBones(std::map<std::string, glm::mat4>& currentPose, Bone* bone, glm::mat4 parentTransform)
	//	{
	//		if (currentPose.find(bone->GetName()) != currentPose.end())
	//		{
	//			glm::mat4 currentLocalTransform = currentPose[bone->GetName()];
	//			glm::mat4 currentTransform = parentTransform * currentLocalTransform;
	//			for (Bone* child : bone->GetChildren())
	//				ApplyPoseToBones(currentPose, child, currentTransform);
	//			currentTransform *= bone->GetInverseBindTransform();
	//			bone->SetAnimationTransform(currentTransform);
	//		}
	//	}

	//	std::vector<KeyFrame*> GetPreviousAndNextFrames()
	//	{
	//		std::vector<KeyFrame>& allFrames = m_CurrentAnimation->GetKeyFrames();
	//		KeyFrame* previousFrame = &allFrames[0];
	//		KeyFrame* nextFrame = &allFrames[0];
	//		for (unsigned int i = 1; i < allFrames.size(); ++i)
	//		{
	//			nextFrame = &allFrames[i];
	//			if (nextFrame->GetTimeStamp() > m_AnimationTime)
	//				break;
	//			previousFrame = &allFrames[i];
	//		}
	//		return std::vector<KeyFrame*> { previousFrame, nextFrame };
	//	}

	//	float CalculateProgression(KeyFrame* previousFrame, KeyFrame* nextFrame) const
	//	{
	//		float totalTime = nextFrame->GetTimeStamp() - previousFrame->GetTimeStamp();
	//		float currentTime = m_AnimationTime - previousFrame->GetTimeStamp();
	//		return currentTime / totalTime;
	//	}

	//	std::map<std::string, glm::mat4> InterpolatePoses(KeyFrame* previousFrame, KeyFrame* nextFrame, float progression)
	//	{
	//		std::map<std::string, glm::mat4> currentPose;
	//		for (auto bone : currentPose)
	//		{
	//			// bone.first is the name of the bone as a string
	//			BoneTransform previousTransform = previousFrame->GetBoneKeyFrames()[bone.first];
	//			BoneTransform nextTransform = nextFrame->GetBoneKeyFrames()[bone.first];
	//			BoneTransform currentTransform = BoneTransform::Interpolate(previousTransform, nextTransform, progression);
	//			currentPose.emplace(bone.first, currentTransform.GetTransformMatrix());
	//		}
	//		return currentPose;
	//	}
	//};
}