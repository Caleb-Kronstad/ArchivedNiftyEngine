#pragma once

namespace Nifty
{

	/*class KeyFrame
	{
	private:
		std::map<std::string, BoneTransform> m_Pose;
		float m_TimeStamp;

	public:
		KeyFrame(float timeStamp, std::map<std::string, BoneTransform> boneKeyFrames)
			: m_TimeStamp(timeStamp), m_Pose(boneKeyFrames)
		{
		}

		float GetTimeStamp() const { return m_TimeStamp; }
		std::map<std::string, BoneTransform>& GetBoneKeyFrames() { return m_Pose; }
	};

	class Animation
	{
	private:
		std::vector<KeyFrame> m_KeyFrames;
		float m_Length = 0.0f;

	public:
		Animation(float lengthInSeconds, std::vector<KeyFrame> keyFrames)
			: m_Length(lengthInSeconds), m_KeyFrames(keyFrames)
		{
		}

		float GetLength() const { return m_Length; }
		std::vector<KeyFrame>& GetKeyFrames() { return m_KeyFrames; }
	};*/
}