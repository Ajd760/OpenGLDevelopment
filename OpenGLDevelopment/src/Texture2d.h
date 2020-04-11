#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <iostream>

#include <glad\glad.h>

class Texture2d
{
public:
	unsigned char* data;
	
	Texture2d();
	Texture2d(std::string filePath);
	~Texture2d();

	void Texture2d::bindTexture();
	void Texture2d::unbindTexture();

	GLuint Texture2d::getID() const;
	GLuint Texture2d::getWidth() const;
	GLuint Texture2d::getHeight() const;
	GLuint Texture2d::getNumChannels() const;

protected:
	GLuint _texture_ID;
	int _width, _height, _num_channels;
};


#endif // !TEXTURE2D_H