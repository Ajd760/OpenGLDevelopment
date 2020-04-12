#version 330 core

in vec3 ourColor;
in vec2 texCoordinate;

uniform sampler2D boxTex;
uniform sampler2D linuxTex;

out vec4 FragColor;

void main()
{
	// the mix() function is a linear interpolation of the 2 given texture colors at the ratio of the 3rd argument
	FragColor = mix(texture(boxTex, texCoordinate), texture(linuxTex, texCoordinate), 0.5);
	//FragColor = texture(smileyTex, texCoordinate);
}