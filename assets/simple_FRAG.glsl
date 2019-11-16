#version 400

uniform vec3 objectColor;
uniform sampler2D diffuse;

in vec2 texCoords0;
vec4 c1;
vec4 c2;


void main()
{
	 
	    c1 = texture2D(diffuse, texCoords0);
		c2 = vec4(objectColor, 1.f) + c1;
		gl_FragColor = clamp(c2, 0.0f, 1.0f); 
}
