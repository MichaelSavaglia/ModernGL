#pragma once

#include <glew.h>

#include <glfw3.h>
class Objects
{
public:
	Objects();
	~Objects();
	GLuint GetVertexBuffer() {return vertexBuffer;};
	GLuint GetUVBuffer(){ return UVBuffer; };
	GLuint GetNormalBuffer(){ return normalBuffer; };
	GLuint GetTextureID(){ return textureID; };
	GLuint GetVertexCount(){ return vertexCount; };

private:
	GLuint vertexBuffer;
	GLuint UVBuffer;
	GLuint normalBuffer;
	GLuint textureID;
	GLuint vertexCount;
};

