#version 400

uniform vec3 objectColor;
uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform int gamma;

in vec2 texCoords0;
vec4 c1;
vec4 c2;

vec3 lightColor = vec3(1.0f,1.0f,1.0f);


in vec3 Normal;
in vec3 FragPos;
float specularStrength = 0.5;


void main()
{
	
   

 

		gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
}
