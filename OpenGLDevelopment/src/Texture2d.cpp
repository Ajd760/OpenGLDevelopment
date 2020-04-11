#include <string>

#include "Texture2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture2d::Texture2d()
{
	
}

Texture2d::Texture2d(std::string filePath, int* width, int* height, int* nrChannels)
{
	// Load texture
}


Texture2d::~Texture2d()
{
	
}

GLuint Texture2d::getID() const
{
	return texture_ID;
}

GLuint Texture2d::getWidth() const
{
	return width;
}

GLuint Texture2d::getHeight() const
{
	return height;
}

GLuint Texture2d::getNumChannels() const
{
	return num_channels;
}