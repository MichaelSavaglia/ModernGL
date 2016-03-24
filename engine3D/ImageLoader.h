#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glew.h>

#include <glfw3.h>
#include <SOIL.h>

namespace Textures
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		unsigned char* data;
		GLuint textureID;
		int width;
		int height;
		
	private:

	};

	Texture* LoadIMG(const char* path);

}