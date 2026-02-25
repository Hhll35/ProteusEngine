#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture1;

void main()
{
	float ambientStr = 0.1;
	vec3 ambient = ambientStr * lightColor;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specStr = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specStr * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	vec4 texColor = texture(texture1, TexCoord);

    FragColor = vec4(result, 1.0) * texColor;
}