#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
	out vec3 FragPos;
	out vec3 Normal;
	out vec2 TexCoords;
	out vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform bool reverse_normals;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	if (reverse_normals)
		vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * aNormal);
	else
		vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.TexCoords = aTexCoords;
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}