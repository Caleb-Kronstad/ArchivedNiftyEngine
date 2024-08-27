#pragma once

#include "nfpch.h"

namespace Nifty {

	struct Transform
	{
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::vec3 EulerAngles; // converts to quaternion when drawing the object

		Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 eulerAngles = glm::vec3(0.0f))
			: Position(position), Scale(scale), EulerAngles(eulerAngles)
		{
		}

		glm::quat GetOrientation() const
		{
			return glm::quat(EulerAngles / 180.0f * glm::pi<float>());
		}

		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 rotation = glm::toMat4(GetOrientation());

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct BoneTransform
	{
		glm::vec3 Position;
		glm::quat Orientation;
		glm::vec3 Scale;

		BoneTransform(glm::vec3 position = glm::vec3(0.0f), glm::quat orientation = glm::quat(), glm::vec3 scale = glm::vec3(1.0f))
			: Position(position), Orientation(orientation), Scale(scale)
		{
		}

		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 rotation = glm::toMat4(Orientation);

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation 
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		static BoneTransform Interpolate(BoneTransform frameA, BoneTransform frameB, float progression)
		{
			glm::vec3 pos = InterpolateTranslations(frameA.Position, frameB.Position, progression);
			glm::quat rot = glm::slerp(frameA.Orientation, frameB.Orientation, progression);
			glm::vec3 sca = InterpolateTranslations(frameA.Scale, frameB.Scale, progression);
			return BoneTransform(pos, rot, sca);
		}

		static glm::vec3 InterpolateTranslations(glm::vec3& start, glm::vec3& end, float progression)
		{
			float x = start.x + (end.x - start.x) * progression;
			float y = start.y + (end.y - start.y) * progression;
			float z = start.z + (end.z - start.z) * progression;
			return glm::vec3(x, y, z);
		}
	};
}