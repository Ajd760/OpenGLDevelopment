#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture()
{
	
}

Texture(std::string filePath, int* width, int* height, int* nrChannels)
{
	// Load texture
}


Texture::~Texture()
{
	
}

GLuint Texture::getID() const
{
	return texture_ID;
}

GLuint Texture::getWidth() const
{
	return width;
}

GLuint Texture::getHeight() const
{
	return height;
}

GLuint Texture::getNumChannels() const
{
	return num_channels;
}