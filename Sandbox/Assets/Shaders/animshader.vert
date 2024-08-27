#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
//layout(location = 3) in vec3 aTangent;
//layout(location = 4) in vec3 aBitangent;
layout(location = 3) in ivec4 aBoneIds; 
layout(location = 4) in vec4 aWeights;

out VS_OUT {
	out vec3 Normal;
	out vec2 TexCoords;
} vs_out;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 3;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 boneTransforms[MAX_BONES];

void main()
{
    vec4 totalLocalPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);

	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		mat4 boneTransform = boneTransforms[aBoneIds[i]];
		vec4 posePosition = boneTransform * vec4(aPosition, 1.0);
		totalLocalPos += posePosition * aWeights[i];

		vec4 worldNormal = boneTransform * vec4(aNormal, 0.0);
		totalNormal += worldNormal * aWeights[i];
	}

	gl_Position = projection * view * model * totalLocalPos;
	vs_out.Normal = totalNormal.xyz;
	vs_out.TexCoords = aTexCoords;
}