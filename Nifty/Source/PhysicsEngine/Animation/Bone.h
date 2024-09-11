#pragma once

namespace Nifty
{
	
	/*class Bone
	{
	private:
		std::vector<Bone*> m_Children;
		unsigned int m_Index;
		std::string m_Name;
		glm::mat4 m_BoneTransform = glm::mat4(1.0);
		glm::mat4 m_LocalBindTransform;
		glm::mat4 m_InverseBindTransform = glm::mat4(1.0);

	public:
		Bone(unsigned int index, std::string name, glm::mat4 bindLocalTransform)
			: m_Index(index), m_Name(name), m_LocalBindTransform(bindLocalTransform)
		{
		}

		void AddChild(Bone* child) { m_Children.push_back(child); }
		void SetAnimationTransform(glm::mat4 animationTransform) { m_BoneTransform = animationTransform; }
		const glm::mat4& GetAnimatedTransform() { return m_BoneTransform; }
		const glm::mat4 GetInverseBindTransform()& { return m_InverseBindTransform; }
		const std::vector<Bone*>& GetChildren() { return m_Children; }
		const std::string GetName()& { return m_Name; }
		unsigned int GetIndex() const { return m_Index; }

		void CalcInverseBindTransform(glm::mat4 parentBindTransform)
		{
			glm::mat4 bindTransform = parentBindTransform * m_LocalBindTransform;
			bindTransform = glm::inverse(bindTransform);
			for (Bone* child : m_Children)
			{
				child->CalcInverseBindTransform(bindTransform);
			}
		}
	};*/
	
}
