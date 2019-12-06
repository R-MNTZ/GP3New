#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 normal;


out vec3 position;

out vec2 texCoords0;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
	
	gl_Position = MVP * vec4(pos, 1.0f);
	texCoords0 = texCoords;
	Normal = normal;
	FragPos = position;
	
}