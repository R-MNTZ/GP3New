#version 330 core

uniform vec3 objectColor;
uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform vec3 viewPos;
//uniform vec3 lightPos;

uniform int gamma;

in vec2 texCoords0;
vec4 c1;


vec3 lightColor = vec3(1.0f,1.0f,1.0f);
vec3 ligtPos = vec3(0 , 10, 0);

in vec3 Normal;
in vec3 FragPos;
float specularStrength = 1.0f;


void main()
{
	
   
    vec3 gammaTex = pow(texture2D(diffuse, texCoords0).rgb, vec3(2.2));
    vec4 c1 = vec4(gammaTex, 1.0f);

	//ambient
	vec3 ambient = 0.7 * lightColor;

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = vec3(0.0f, 10.0f, 0.f); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse1 = diff * lightColor;

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

		
		
	   
	   vec3 tempColor = (ambient + diffuse1) * c1.rgb;
		vec4 fragColor = vec4(tempColor, 1.0) + vec4(objectColor, 1.0);

		

		
		if (gamma == 1){
		float gamma = 2.2;
		fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));

		} 

		gl_FragColor = clamp(fragColor, 0.0f, 1.0f); 
}
