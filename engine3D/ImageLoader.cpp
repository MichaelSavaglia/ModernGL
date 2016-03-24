#include "ImageLoader.h"




namespace Textures
{
	Texture::Texture()
	{

	}

	Texture::~Texture()
	{

	}

	Texture* LoadIMG(const char* path)
	{
		Texture* tempTex = new Texture();
		tempTex->data = SOIL_load_image(path, &tempTex->width, &tempTex->height, 0, SOIL_LOAD_RGBA);

		glGenTextures(1, &tempTex->textureID);
		glBindTexture(GL_TEXTURE_2D, tempTex->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempTex->width, tempTex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempTex->data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		return tempTex;
	}

}