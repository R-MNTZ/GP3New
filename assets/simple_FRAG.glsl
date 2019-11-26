#version 400

uniform vec3 objectColor;
uniform sampler2D diffuse;
uniform vec3 viewPos;
//uniform vec3 lightPos;

in vec2 texCoords0;
vec4 c1;
vec4 c2;
vec3 lightColor = vec3(1.0f,1.0f,1.0f);
vec3 lightPos = vec3 (10.0f, 0.0f, 0.0f);
in vec3 Normal;
in vec3 FragPos;
float specularStrength = 0.5;


void main()
{
    vec4 color1 = texture2D(diffuse, texCoords0);
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse1 = diff * lightColor;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

		
		vec3 ambient = 0.1 * color1.rgb;
	   
		c2 = vec4(((ambient + specular + diffuse1) + color1.rgb), 1.f);

		gl_FragColor = clamp(c2, 0.0f, 1.0f); 
}
