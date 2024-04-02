#version 460 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	sampler2D height;
	float shininess;
};

struct DirLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT {
	in vec3 FragPos;
	in vec3 Normal;
	in vec2 TexCoords;
} fs_in;

uniform vec3 viewPos;
uniform Material material;

uniform int numPointLights;
uniform int numSpotLights;

uniform DirLight dirLight; 
uniform PointLight pointLights[15];
uniform SpotLight spotLights[15];

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float gamma = 1.0;

void main()
{
	// properties
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// direction
	vec3 result;
	result += CalculateDirectionalLight(dirLight, norm, viewDir);

	// point
	for (int i = 0; i < numPointLights; i++){
		result += CalculatePointLight(pointLights[i], norm, fs_in.FragPos, viewDir);
	}

	// spot
	for (int i = 0; i < numSpotLights; i++){
		result += CalculateSpotLight(spotLights[i], norm, fs_in.FragPos, viewDir);
	}

	vec4 fragColor = vec4(result, 1.0);
	FragColor = fragColor;
	
	FragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
	// directional light calculations
	vec3 lightDir = normalize(light.direction - fs_in.FragPos);
	vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);

	// specular
		// phong
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// blinn-phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	// combine results
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * pow(color, vec3(gamma));
	vec3 specular = light.specular * spec * color;

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	// point light calculations
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;

	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	
	// specular
		// phong
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// blinn-phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	// attenuation
	float dist = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	
	// combine results
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * pow(color, vec3(gamma));
	vec3 specular = light.specular * spec * color;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	// spot light calculations
	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;

	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	// specular
		// phong
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// blinn-phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	// soft edges
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / epsilon);

	// attenuation
	float dist = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	
	// combine results
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * pow(texture(material.diffuse, fs_in.TexCoords).rgb, vec3(gamma));
	vec3 specular = light.specular * spec * color;

	diffuse *= intensity;
	specular *= intensity;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}