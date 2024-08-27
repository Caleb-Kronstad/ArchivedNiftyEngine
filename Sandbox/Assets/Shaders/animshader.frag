#version 460 core
out vec4 FragColor;

in VS_OUT {
	in vec3 Normal;
	in vec2 TexCoords;
} fs_in;

struct Material {
	sampler2D diffuse;
};

const vec2 lightBias = vec2(0.7, 0.6);

uniform Material material;
uniform vec3 lightDirection;

void main()
{    
	vec4 diffuseColor = texture(material.diffuse, fs_in.TexCoords);
	vec3 unitNormal = normalize(fs_in.Normal);
	float diffuseLight = max(dot(-lightDirection, unitNormal), 0.0) * lightBias.x + lightBias.y;
    FragColor = diffuseColor * diffuseLight;
}
