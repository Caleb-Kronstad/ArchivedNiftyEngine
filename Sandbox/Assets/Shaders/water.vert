#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float wavelength;
uniform float amplitude;
uniform float speed;
uniform float time;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	float newPosX; 
	for (int i = 1; i < 2; i++){
		float frequency = 2 / (wavelength * i);
		float faze = speed * frequency;
		newPosX *= amplitude * sin((aPos.x * frequency) + (time * faze));
	}

	vec3 newPos = vec3(newPosX, aPos.y, aPos.z);

	gl_Position = projection * view * model * vec4(newPos, 1.0);
	FragPos = vec3(model * vec4(newPos, 1.0));
	Normal = aNormal;
	TexCoords = aTexCoords;
}